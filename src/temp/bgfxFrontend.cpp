// bgfxFrontend.cpp
//

#include "bgfxFrontend.hh"
#define LZZ_INLINE inline
bgfx_stb_prerendered_text::bgfx_stb_prerendered_text ()
  : sttfont_prerendered_text (), mBgfxTexture (BGFX_INVALID_HANDLE)
                                                                                                     {}
void bgfx_stb_prerendered_text::freeTexture ()
                            {
		if (bgfx::isValid(mBgfxTexture))
			bgfx::destroy(mBgfxTexture);
		mBgfxTexture = BGFX_INVALID_HANDLE;
		}
int bgfx_stb_prerendered_text::draw (bgfx::ViewId mViewId, int const x, int const y)
                                                                  {
		return draw_worker(mViewId, x, y, true);
		}
int bgfx_stb_prerendered_text::draw_worker (bgfx::ViewId mViewId, int const x, int const y, bool const resetColour)
                                                                                                 {
		if (!width) return 0; // don't print null texture
		bgfxsfh::rect r;
		r.x = x;
		r.y = y;
		r.w = width;
		r.h = height;
		
		bgfxsfh::rect rt;
		rt.x = 0;
		rt.y = 0;
		rt.w = 1;
		rt.h = 1;
		
		if (resetColour)
			bgfx::setUniform(bgfxsfh::u_colour, bgfxsfh::toVec4(255,255,255,255).v);
		bgfx::setTexture(0, bgfxsfh::s_texture, mBgfxTexture);
		bgfxsfh::pushTexturedQuad(r, rt);
		bgfx::setState(bgfxsfh::RENDER_STATE);
		bgfx::submit(mViewId, bgfxsfh::texturedProgram);
		
		//std::cout << "drawing prerendered!!! " << x << ", " << y << ", "  << width << ", " << height << std::endl;
		BGFXSFH_IS_VALID(mBgfxTexture);
		return r.x + r.w;
		}
int bgfx_stb_prerendered_text::drawWithColorMod (bgfx::ViewId mViewId, int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a)
                                                                                                                                                       {
		bgfx::setUniform(bgfxsfh::u_colour, bgfxsfh::toVec4(r,g,b,a).v);
		return draw_worker(mViewId, x, y, false);
		}
bgfx::ShaderHandle bgfxsfh::vert_passthrough = BGFX_INVALID_HANDLE;
bgfx::ShaderHandle bgfxsfh::frag_passthrough = BGFX_INVALID_HANDLE;
bgfx::ShaderHandle bgfxsfh::textured_vert_passthrough = BGFX_INVALID_HANDLE;
bgfx::ShaderHandle bgfxsfh::textured_frag_passthrough = BGFX_INVALID_HANDLE;
bgfx::ProgramHandle bgfxsfh::untexturedProgram = BGFX_INVALID_HANDLE;
bgfx::ProgramHandle bgfxsfh::texturedProgram = BGFX_INVALID_HANDLE;
bgfx::UniformHandle bgfxsfh::u_colour = BGFX_INVALID_HANDLE;
bgfx::UniformHandle bgfxsfh::s_texture = BGFX_INVALID_HANDLE;
uint32_t const bgfxsfh::RENDER_STATE = BGFX_STATE_WRITE_RGB|BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
uint32_t const bgfxsfh::RENDER_STATE_PRERENDER = BGFX_STATE_WRITE_RGB|BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_EQUATION(BGFX_STATE_BLEND_EQUATION_ADD);
int bgfxsfh::refCount = 0;
bgfxsfh::Vec4 bgfxsfh::toVec4 (uint8_t const a, uint8_t const b, uint8_t const c, uint8_t const d)
                                                                                               {
		return Vec4::fromUint8(a,b,c,d);
		}
void bgfxsfh::initialise ()
                                 {
		if (refCount == 0) {
			PosVertex::init();
			PosTexCoord0Vertex::init();
		
			#include "bgfxFrontendShaders.h"
			
			static const bgfx::EmbeddedShader s_embeddedShaders[] = {
				BGFXH_EMBEDDED_SHADER(fs_textured_passthrough_bin),
				BGFXH_EMBEDDED_SHADER(vs_textured_passthrough_bin),
				BGFXH_EMBEDDED_SHADER(fs_untextured_passthrough_bin),
				BGFXH_EMBEDDED_SHADER(vs_untextured_passthrough_bin),
				
				BGFX_EMBEDDED_SHADER_END()
				};
				
			
			bgfx::RendererType::Enum type = bgfx::getRendererType();
			vert_passthrough = bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_untextured_passthrough_bin");
			frag_passthrough = bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_untextured_passthrough_bin");
			textured_vert_passthrough = bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_textured_passthrough_bin");
			textured_frag_passthrough = bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_textured_passthrough_bin");
			
			untexturedProgram = bgfx::createProgram(vert_passthrough, frag_passthrough, false);
			texturedProgram = bgfx::createProgram(textured_vert_passthrough, textured_frag_passthrough, false);
			
			u_colour = bgfx::createUniform("u_colour", bgfx::UniformType::Vec4);
			s_texture = bgfx::createUniform("s_texture", bgfx::UniformType::Sampler);
			
			BGFXSFH_IS_VALID(vert_passthrough);
			BGFXSFH_IS_VALID(frag_passthrough);
			BGFXSFH_IS_VALID(textured_vert_passthrough);
			BGFXSFH_IS_VALID(textured_frag_passthrough);
			BGFXSFH_IS_VALID(untexturedProgram);
			BGFXSFH_IS_VALID(texturedProgram);
			
			BGFXSFH_IS_VALID(u_colour);
			BGFXSFH_IS_VALID(s_texture);
			}
		refCount++;
		}
void bgfxsfh::deinitialise ()
                                   {
		refCount--;
		if (refCount == 0) {
			bgfx::destroy(untexturedProgram); 	untexturedProgram = BGFX_INVALID_HANDLE;
			bgfx::destroy(texturedProgram); 	texturedProgram = BGFX_INVALID_HANDLE;
			
			bgfx::destroy(vert_passthrough); 	vert_passthrough = BGFX_INVALID_HANDLE;
			bgfx::destroy(frag_passthrough); 	frag_passthrough = BGFX_INVALID_HANDLE;
			bgfx::destroy(textured_vert_passthrough); textured_vert_passthrough = BGFX_INVALID_HANDLE;
			bgfx::destroy(textured_frag_passthrough); textured_frag_passthrough = BGFX_INVALID_HANDLE;
			
			bgfx::destroy(u_colour); 			u_colour = BGFX_INVALID_HANDLE;
			bgfx::destroy(s_texture); 			s_texture = BGFX_INVALID_HANDLE;
			}
		}
void bgfxsfh::PosTexCoord0Vertex::init ()
                                   {
			ms_decl.begin()
				.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
			}
bgfx::VertexLayout bgfxsfh::PosTexCoord0Vertex::ms_decl;
void bgfxsfh::PosVertex::init ()
                                   {
			ms_decl.begin()
				.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
			.end();
			}
bgfx::VertexLayout bgfxsfh::PosVertex::ms_decl;
void bgfxsfh::pushTexturedQuad (rect const & r, rect const & r2, bool dontFilpY)
                                                                                              { pushTexturedQuad(r.x, r.y, r.w, r.h, r2.x, r2.y, r2.w, r2.h, dontFilpY); }
void bgfxsfh::pushTexturedQuad (float const xOffset, float const yOffset, float const xSize, float const ySize, float const texU, float const texV, float const uWidth, float const vHeight, bool const dontFilpY, float const _framebufferWidth, float const _framebufferHeight)
                                                                                                                                                                                                                                                                                                   {
		/*
		* From the BGFX Examples, the following license applies to only this function:
		* Copyright 2011-2018 Branimir Karadzic. All rights reserved.
		* License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
		*/
		const bgfx::RendererType::Enum renderer = bgfx::getRendererType();
		const float _texelHalf = bgfx::RendererType::Direct3D9 == renderer ? 0.5f : 0.0f;
		bool _originBottomLeft = false; 
		if (!dontFilpY)
			_originBottomLeft = bgfx::getCaps()->originBottomLeft; // Prevent double flipping
		
		if (6 == bgfx::getAvailTransientVertexBuffer(6, bgfxsfh::PosTexCoord0Vertex::ms_decl) ) {
			bgfx::TransientVertexBuffer vb;
			bgfx::allocTransientVertexBuffer(&vb, 6, bgfxsfh::PosTexCoord0Vertex::ms_decl);
			bgfxsfh::PosTexCoord0Vertex* vertex = (bgfxsfh::PosTexCoord0Vertex*)vb.data;

			const float minx = xOffset;
			const float maxx = (xOffset + xSize);
			const float miny = yOffset;
			const float maxy = (yOffset + ySize);

			float texelHalfW = 0.f;
			float texelHalfH = 0.f;
			if (_framebufferWidth > 0.0f && _framebufferHeight > 0.0f) {
				texelHalfW = _texelHalf/_framebufferWidth;
				texelHalfH = _texelHalf/_framebufferHeight;
				}
			const float minu = texU + texelHalfW;
			const float maxu = texU+uWidth + texelHalfH;

			const float zz = 0.0f;

			float minv = texV + texelHalfH;
			float maxv = texV+vHeight + texelHalfH;

			if (_originBottomLeft) {
				float temp = minv;
				minv = maxv;
				maxv = temp;
				}

			vertex[0].m_x = minx;
			vertex[0].m_y = miny;
			vertex[0].m_z = zz;
			vertex[0].m_u = minu;
			vertex[0].m_v = minv;

			vertex[1].m_x = maxx;
			vertex[1].m_y = miny;
			vertex[1].m_z = zz;
			vertex[1].m_u = maxu;
			vertex[1].m_v = minv;

			vertex[2].m_x = maxx;
			vertex[2].m_y = maxy;
			vertex[2].m_z = zz;
			vertex[2].m_u = maxu;
			vertex[2].m_v = maxv;
			
			vertex[3].m_x = minx;
			vertex[3].m_y = miny;
			vertex[3].m_z = zz;
			vertex[3].m_u = minu;
			vertex[3].m_v = minv;

			vertex[4].m_x = maxx;
			vertex[4].m_y = maxy;
			vertex[4].m_z = zz;
			vertex[4].m_u = maxu;
			vertex[4].m_v = maxv;
			
			vertex[5].m_x = minx;
			vertex[5].m_y = maxy;
			vertex[5].m_z = zz;
			vertex[5].m_u = minu;
			vertex[5].m_v = maxv;


			bgfx::setVertexBuffer(0, &vb);
			}
		}
void bgfxsfh::pushUntexturedQuad (rect const & r)
                                                       { pushUntexturedQuad(r.x, r.y, r.w, r.h); }
void bgfxsfh::pushUntexturedQuad (float const xOffset, float const yOffset, float const xSize, float const ySize, float const _framebufferWidth, float const _framebufferHeight)
                                                                                                                                                                                                  {
		/*
		* From the BGFX Examples, the following license applies to only this function:
		* Copyright 2011-2018 Branimir Karadzic. All rights reserved.
		* License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
		*/
		const bgfx::RendererType::Enum renderer = bgfx::getRendererType();
		const float _texelHalf = bgfx::RendererType::Direct3D9 == renderer ? 0.5f : 0.0f;
		const bool _originBottomLeft = bgfx::getCaps()->originBottomLeft;
		
		if (6 == bgfx::getAvailTransientVertexBuffer(6, bgfxsfh::PosVertex::ms_decl) ) {
			bgfx::TransientVertexBuffer vb;
			bgfx::allocTransientVertexBuffer(&vb, 6, bgfxsfh::PosVertex::ms_decl);
			bgfxsfh::PosVertex* vertex = (bgfxsfh::PosVertex*)vb.data;

			const float minx = xOffset;
			const float maxx = (xOffset + xSize);
			const float miny = yOffset;
			const float maxy = (yOffset + ySize);

			float texelHalfW = 0.f;
			float texelHalfH = 0.f;
			if (_framebufferWidth > 0.0f && _framebufferHeight > 0.0f) {
				texelHalfW = _texelHalf/_framebufferWidth;
				texelHalfH = _texelHalf/_framebufferHeight;
				}
			const float minu = 0.0f + texelHalfW;
			const float maxu = 1.0f + texelHalfH;

			const float zz = 0.0f;

			float minv = texelHalfH;
			float maxv = 1.0f + texelHalfH;

			if (_originBottomLeft) {
				float temp = minv;
				minv = maxv;
				maxv = temp;
				}

			vertex[0].m_x = minx;
			vertex[0].m_y = miny;
			vertex[0].m_z = zz;

			vertex[1].m_x = maxx;
			vertex[1].m_y = miny;
			vertex[1].m_z = zz;

			vertex[2].m_x = maxx;
			vertex[2].m_y = maxy;
			vertex[2].m_z = zz;
			
			vertex[3].m_x = minx;
			vertex[3].m_y = miny;
			vertex[3].m_z = zz;

			vertex[4].m_x = maxx;
			vertex[4].m_y = maxy;
			vertex[4].m_z = zz;
			
			vertex[5].m_x = minx;
			vertex[5].m_y = maxy;
			vertex[5].m_z = zz;


			bgfx::setVertexBuffer(0, &vb);
			}
		}
bgfx_stb_glyph::bgfx_stb_glyph ()
  : sttfont_glyph (), mAtlasTexture (BGFX_INVALID_HANDLE), x (0), y (0), w (1), h (1)
                                                                                                        {}
bgfx_stb_glyph_atlas::bgfx_stb_glyph_atlas ()
  : mAtlasTexture (BGFX_INVALID_HANDLE), mStbRectPackCtx (NULL), mNodes (NULL), isFull (false)
                                                                                                                        {}
bgfx_stb_font_cache::bgfx_stb_font_cache ()
  : sttfont_font_cache (), mViewId (0), mAtlasSize (1024), isRenderingToTarget (false)
                                                                                                                {}
bgfx_stb_font_cache::~ bgfx_stb_font_cache ()
                                {		
		clearGlyphs();
		bgfxsfh::deinitialise();
		}
void bgfx_stb_font_cache::clearGlyphs ()
                            {
		for (auto & g : mAtlases) {
			bgfx::destroy(g.mAtlasTexture);
			SSF_DEL_ARR(g.mNodes);
			SSF_DEL(g.mStbRectPackCtx);
			}
			
		mAtlases.clear();
		mGlyphs.clear();
		}
void bgfx_stb_font_cache::bindRenderer (bgfx::ViewId const _viewId)
                                                      {
		mViewId = _viewId;
		bgfxsfh::initialise();
		}
bgfx_stb_glyph_atlas * bgfx_stb_font_cache::getGenAtlasPage ()
                                                {
		if (mAtlases.empty()) {
			return createAtlasPage();
			}
		bgfx_stb_glyph_atlas * r = &mAtlases.back();
		if (r->isFull)
			return createAtlasPage();
		return r;
		}
bgfx_stb_glyph_atlas * bgfx_stb_font_cache::createAtlasPage ()
                                                {
		bgfx_stb_glyph_atlas a;
		a.mAtlasTexture = bgfx::createTexture2D(mAtlasSize, mAtlasSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_NONE, NULL);
		
		/*	
		const bgfx::Memory* mem = bgfx::alloc(mAtlasSize*mAtlasSize*4);
		memset(mem->data, 0, mem->size);
		for (uint32_t i = 0; i < mem->size; ++i) {
			mem->data[i] = 255;//(i^1337)%255;
			}
		bgfx::updateTexture2D(a.mAtlasTexture, 0, 0, 0, 0, mAtlasSize, mAtlasSize, mem, mAtlasSize);
		bgfx::frame();
		bgfx::frame();
		bgfx::frame();
		bgfx::frame();*/
		
		int nNodes = mAtlasSize;
		a.mStbRectPackCtx = SSF_NEW(stbrp_context);
		a.mNodes = SSF_NEW_ARR(stbrp_node, nNodes);
		
		for (int i = 0; i < nNodes; ++i) {
			a.mNodes[i].x = 0;
			a.mNodes[i].y = 0;
			a.mNodes[i].next = NULL;
			}
		
		stbrp_init_target(a.mStbRectPackCtx, mAtlasSize, mAtlasSize, a.mNodes, nNodes);
		
		mAtlases.push_back(a);
		return &mAtlases.back();
		}
void bgfx_stb_font_cache::pregenGlyphs (SSF_VECTOR <sttfont_uint32_t_range> & mRanges, uint8_t const format)
                                                                                              {
		// Make your own implmentation for your own frontend here
		SSF_VECTOR<tempGlyph> tempGlyphs;
		SSF_VECTOR<stbrp_rect> rects;
		
		uint32_t size = 0;
		for (sttfont_uint32_t_range & sur : mRanges)
			size += sur.end - sur.start + 1;
		
		tempGlyphs.reserve(size);
		rects.reserve(size);
		
		for (sttfont_uint32_t_range & sur : mRanges) {
			for (uint32_t codepoint = sur.start; codepoint <= sur.end; ++codepoint) {
				uint64_t target = codepoint | (uint64_t(format & ~(sttfont_format::FORMAT_STRIKETHROUGH | sttfont_format::FORMAT_UNDERLINE)) << 32);
				
				// does the codepoint exist already?
				auto check = mGlyphs.find(target);
				if (check != mGlyphs.end()) continue;
				
				bgfx_stb_glyph g;
				tempGlyph t;
				stbrp_rect r;
				
				t.bitmapData = NULL;
				
				genGlyph(codepoint, format, &g, &t.bitmapData);
				
				if (t.bitmapData) {
					r.id = *((int*) &codepoint);
					r.x = 0;
					r.y = 0;
					r.w = g.width;
					r.h = g.height;
					r.was_packed = 0;
					
					mGlyphs[target] = std::move(g);
					t.target = target;
					
					tempGlyphs.push_back(std::move(t));
					rects.push_back(std::move(r));
					}
				}
			}
		
		pregenGlyphs_pack(tempGlyphs, rects, true);
		}
void bgfx_stb_font_cache::pregenGlyphs_pack (SSF_VECTOR <tempGlyph> & tempGlyphs, SSF_VECTOR <stbrp_rect> & rects, bool force)
                                                                                                               {
		bgfx_stb_glyph_atlas* activeAtlas = getGenAtlasPage();
		stbrp_pack_rects(activeAtlas->mStbRectPackCtx, rects.data(), rects.size());
		
		SSF_VECTOR<tempGlyph> glyphs_rejected;
		SSF_VECTOR<stbrp_rect> rects_rejected;
		
		bool anyNotPacked = false;
		
		for (unsigned int i = 0; i < rects.size(); ++i) {
			stbrp_rect & r = rects[i];
			tempGlyph & t = tempGlyphs[i];
			bgfx_stb_glyph * bOut = (bgfx_stb_glyph*) &mGlyphs[t.target];
			
			if (r.was_packed) {
				const bgfx::Memory* mem = bgfx::copy(t.bitmapData, bOut->width*bOut->height*4);
				bgfx::updateTexture2D(activeAtlas->mAtlasTexture, 0, 0, r.x, r.y, r.w, r.h, mem);
				bOut->x = r.x/float(mAtlasSize);
				bOut->y = r.y/float(mAtlasSize);
				bOut->w = r.w/float(mAtlasSize);
				bOut->h = r.h/float(mAtlasSize); 
				bOut->mAtlasTexture = activeAtlas->mAtlasTexture;
				}
			else {
				glyphs_rejected.push_back(std::move(t));
				rects_rejected.push_back(std::move(r));
				anyNotPacked = true;
				}
			}
		if (anyNotPacked)
			activeAtlas->isFull = true;
		
		//std::cout << " pregenGlyphs_pack " << tempGlyphs.size() << " " << glyphs_rejected.size() << std::endl;
		
		if ((glyphs_rejected.size() != tempGlyphs.size() || force) && glyphs_rejected.size())
			pregenGlyphs_pack(glyphs_rejected, rects_rejected, false);
		}
void bgfx_stb_font_cache::genGlyph_writeData (uint32_t const codepoint, sttfont_glyph * gOut, unsigned char * bitmap2, int w, int h)
                                                                                                                        {
		genGlyph_writeData2(codepoint, gOut, bitmap2, w, h, true);
		}
void bgfx_stb_font_cache::genGlyph_writeData2 (uint32_t const codepoint, sttfont_glyph * gOut, unsigned char * bitmap2, int w, int h, bool firstCall)
                                                                                                                                         {
		// fetch or create atlas
		bgfx_stb_glyph * bOut = (bgfx_stb_glyph*) gOut;
		bgfx_stb_glyph_atlas * activeAtlas = getGenAtlasPage();
		
		std::cout << "genGlyph_writeData2 " << char(codepoint) << " #" << codepoint << ", firstCall: " << firstCall << ", activeAtlas "<< activeAtlas << std::endl;
		
		// try to pack
		stbrp_rect r;
		r.id = *((int*) &codepoint);
		r.x = 0;
		r.y = 0;
		r.w = w;
		r.h = h;
		r.was_packed = 0;
		
		stbrp_pack_rects(activeAtlas->mStbRectPackCtx, &r, 1);
		if (r.was_packed) {
			std::cout << "packing: " << r.x << ", " << r.y << ", " << r.w << ", " << r.h << std::endl;
			const bgfx::Memory* mem = bgfx::copy(bitmap2, w*h*4);
			bgfx::updateTexture2D(activeAtlas->mAtlasTexture, 0, 0, r.x, r.y, r.w, r.h, mem);
			bOut->x = r.x/float(mAtlasSize);
			bOut->y = r.y/float(mAtlasSize);
			bOut->w = r.w/float(mAtlasSize);
			bOut->h = r.h/float(mAtlasSize); 
			bOut->mAtlasTexture = activeAtlas->mAtlasTexture;
			BGFXSFH_IS_VALID(bOut->mAtlasTexture);
			return;
			}
		else {
			activeAtlas->isFull = true;
			}
		
		// failed to pack, generate a new atlas
		if (firstCall)
			genGlyph_writeData2(codepoint, gOut, bitmap2, w, h, false);
		}
sttfont_glyph * bgfx_stb_font_cache::getGlyph (uint64_t const target)
                                                        {
		auto it = mGlyphs.find(target);
		if (it == mGlyphs.end())
			return NULL;
		return &((*it).second);
		}
sttfont_glyph * bgfx_stb_font_cache::genGlyph_createAndInsert (uint64_t const target, uint32_t const codepoint, uint8_t const format)
                                                                                                                        {
		bgfx_stb_glyph g;
		genGlyph(codepoint, format, &g);
		mGlyphs[target] = g;
		return getGlyph(target);
		}
void bgfx_stb_font_cache::processCodepoint (int & x, int & y, uint32_t const codepoint, sttfont_format const * const format, bool isDrawing, int kerningAdv, int & overdraw)
                                                                                                                                                                {
		// Draws the character, advances x & y to the next position
		// NOTE: KErning
		const uint64_t RSTATE = isRenderingToTarget ? bgfxsfh::RENDER_STATE_PRERENDER : bgfxsfh::RENDER_STATE;
		
		uint8_t formatCode = 0;
		if (format)
			formatCode = format->format;
			
		bgfx_stb_glyph * G = (bgfx_stb_glyph*) getGlyphOrTofu(codepoint, formatCode);
		if (!G) {
			x += faceSize/2;
			return;
			}
		if (isDrawing) {
			bgfxsfh::rect r;	// render coords
			r.x = x + G->xOffset;
			r.y = y + G->yOffset + baseline;
			r.w = G->width;
			r.h = G->height;
			
			bgfxsfh::rect rt;	// glyph texcoords
			rt.x = G->x;
			rt.y = G->y;
			rt.w = G->w;
			rt.h = G->h;
			
			if (true) {//bgfx::isValid(G->mAtlasTexture)) {
				if (format) {
					int charAdv = kerningAdv + G->xOffset;
					bool isColoured = (format->r < 255) || (format->g < 255) || (format->b < 255);
					uint8_t cr,cg,cb,ca;
					
					
					if (false) { //isColoured || formatCode) { 
						// Remove bleeding pixels
						// not needed with the bgfx frontend
						bgfx::setUniform(bgfxsfh::u_colour, bgfxsfh::toVec4(format->r, format->g, format->b, 0).v);
						bgfxsfh::rect r2;
						r2.x = r.x; r2.y = r.y;
						r2.w = r.w; r2.h = r.h;
						if (r2.x < overdraw) {
							int dx = overdraw - r2.x;
							r2.x += dx;
							r2.w -= dx;
							}
						overdraw = r.x + r.w;
						
						bgfxsfh::pushUntexturedQuad(r2); //TODO: prevent overlapping!
						bgfx::setState(bgfxsfh::RENDER_STATE);
						bgfx::submit(mViewId, bgfxsfh::untexturedProgram);
						}
					bgfx::setUniform(bgfxsfh::u_colour, bgfxsfh::toVec4(format->r, format->g, format->b, format->a).v);
					bgfx::setTexture(0, bgfxsfh::s_texture, G->mAtlasTexture);
					bgfxsfh::pushTexturedQuad(r, rt, true);
					bgfx::setState(RSTATE);
					bgfx::submit(mViewId, bgfxsfh::texturedProgram);
					
					if (formatCode & sttfont_format::FORMAT_STRIKETHROUGH) {
						bgfxsfh::rect r2;
						r2.w = G->width+strikethroughThickness + charAdv; r2.h = strikethroughThickness;
						if (r2.h < 1) r2.h = 1;
						r2.x = r.x-strikethroughThickness/2 - charAdv; r2.y = y + strikethroughPosition;
						
						bgfxsfh::pushUntexturedQuad(r2);
						bgfx::setState(bgfxsfh::RENDER_STATE);
						bgfx::submit(mViewId, bgfxsfh::untexturedProgram);
						}
					if (formatCode & sttfont_format::FORMAT_UNDERLINE) {
						bgfxsfh::rect r2;
						r2.w = G->width+underlineThickness + charAdv; r2.h = underlineThickness;
						if (r2.h < 1) r2.h = 1;
						r2.x = r.x-underlineThickness/2 - charAdv; r2.y = y + underlinePosition;
						
						bgfxsfh::pushUntexturedQuad(r2);
						bgfx::setState(bgfxsfh::RENDER_STATE);
						bgfx::submit(mViewId, bgfxsfh::untexturedProgram);
						}
					}
				else {
					static int ii = 0;
					bgfx::setTexture(ii, bgfxsfh::s_texture, G->mAtlasTexture);
					ii++;
					if (ii > 4) ii = 0;
					bgfxsfh::Vec4 temp = bgfxsfh::toVec4(255,255,255,255);
					bgfx::setUniform(bgfxsfh::u_colour, temp.v);
					
					overdraw = SSF_INT_MIN;
					bgfxsfh::pushTexturedQuad(r, rt, true);
					bgfx::setState(RSTATE);
					bgfx::submit(mViewId, bgfxsfh::texturedProgram);
					//std::cout << "submitting! " << char(codepoint) << std::endl;
					}
				}
			}
		x += scale*G->advance;
		}
bgfx::TextureHandle bgfx_stb_font_cache::renderTextToTexture (char const * c, uint32_t const maxLen, int * widthOut, int * heightOut)
                                                                                                                                            {
		return renderTextToTexture_worker(NULL, c, maxLen, widthOut, heightOut);
		}
bgfx::TextureHandle bgfx_stb_font_cache::renderTextToTexture (sttfont_formatted_text const & formatted, int * widthOut, int * heightOut)
                                                                                                                                          {
		return renderTextToTexture_worker(&formatted, NULL, -1, widthOut, heightOut);
		}
bgfx::TextureHandle bgfx_stb_font_cache::renderTextToTexture_worker (sttfont_formatted_text const * formatted, char const * c, uint32_t const maxLen, int * widthOut, int * heightOut)
                                                                                                                                                                                             {
		int width, height;
		if (formatted)
			getTextSize(width, height, *formatted);
		else
			getTextSize(width, height, c, maxLen);
			
		if (width < 1) {
			return BGFX_INVALID_HANDLE; // empty string
			}
			
		bgfx::ViewId prv = 0;
			
		
		bgfx::FrameBufferHandle FB = bgfx::createFrameBuffer(width, height, bgfx::TextureFormat::BGRA8, BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP);
		bgfx::TextureHandle RT = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_BLIT_DST, NULL);
		
		bgfx::setViewFrameBuffer(prv, FB);
		float orthoProj[16];	// Ortho matrix for rendering screenspace quads
		bx::mtxOrtho(orthoProj, 0.0f, width, height, 0.0f, 0.0f, 100.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewRect(prv, 0, 0, width, height);
		bgfx::setViewTransform(prv, NULL, orthoProj);
		
		bgfx::setViewClear(prv, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x00000000, 1.0f, 0);
		bgfx::setViewMode(prv, bgfx::ViewMode::Sequential);
		bgfx::touch(prv);
		bgfx::frame();	// needed here to flush & clear the screen
		
		isRenderingToTarget = true;
		if (formatted)
			drawText(0, 0, *formatted);
		else
			drawText(0, 0, c, maxLen);
		isRenderingToTarget = false;
		
		bgfx::blit(prv+1, RT, 0,0, bgfx::getTexture(FB, 0));
		bgfx::touch(prv);
		bgfx::frame();	// needed here to execute the draw call & reset
		bgfx::destroy(FB);
		
		// TBD: reuse framebuffer
		
		std::cout << "Prerendering done! " << bgfx::isValid(RT) << " " << width << ", " << height << " caps " << (bgfx::getCaps()->supported & BGFX_CAPS_TEXTURE_BLIT)  << std::endl;
		
		*widthOut = width;
		*heightOut = height;
		return RT;
		}
bgfx::TextureHandle bgfx_stb_font_cache::renderTextToTexture (SSF_STRING const & str, int * widthOut, int * heightOut)
                                                                                                                        {
		return renderTextToTexture(str.data(), str.length(), widthOut, heightOut);
		}
void bgfx_stb_font_cache::renderTextToObject (sttfont_prerendered_text * textOut, char const * c, uint32_t const maxLen)
                                                                                                                 {
		bgfx_stb_prerendered_text * textOut2 = (bgfx_stb_prerendered_text*) textOut;
		textOut2->mBgfxTexture = renderTextToTexture(c, maxLen, &(textOut->width), &(textOut->height));
		}
void bgfx_stb_font_cache::renderTextToObject (sttfont_prerendered_text * textOut, SSF_STRING const & str)
                                                                                             {
		bgfx_stb_prerendered_text * textOut2 = (bgfx_stb_prerendered_text*) textOut;
		textOut2->mBgfxTexture = renderTextToTexture(str, &(textOut->width), &(textOut->height));
		}
void bgfx_stb_font_cache::renderTextToObject (sttfont_prerendered_text * textOut, sttfont_formatted_text const & str)
                                                                                                         {
		bgfx_stb_prerendered_text * textOut2 = (bgfx_stb_prerendered_text*) textOut;
		textOut2->mBgfxTexture = renderTextToTexture(str, &(textOut->width), &(textOut->height));
		}
#undef LZZ_INLINE
