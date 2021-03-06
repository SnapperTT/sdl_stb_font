// sdlFrontend.cpp
//

#include "sdlFrontend.hh"
#include <SDL2/SDL.h>
#define LZZ_INLINE inline
sdl_stb_prerendered_text::sdl_stb_prerendered_text ()
  : sttfont_prerendered_text (), mSdlTexture (NULL), mRenderer (NULL)
                                                                                                     {}
void sdl_stb_prerendered_text::freeTexture ()
                            {
		if (mSdlTexture)
			SDL_DestroyTexture(mSdlTexture);
		mSdlTexture = NULL;
		}
int sdl_stb_prerendered_text::draw (int const x, int const y)
                                            {
		SDL_Rect r;
		r.x = x;
		r.y = y;
		r.w = width;
		r.h = height;
		SDL_RenderCopy(mRenderer, mSdlTexture, NULL, &r);
		return r.x + r.w;
		}
int sdl_stb_prerendered_text::drawWithColorMod (int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a)
                                                                                                                                 {
		SDL_SetTextureColorMod(mSdlTexture, r, g, b);
		if (a != 255)
			SDL_SetTextureAlphaMod(mSdlTexture, a);
		return draw (x, y);
		}
sdl_stb_glyph::sdl_stb_glyph ()
  : sttfont_glyph (), mSdlTexture (0), mSdlSurface (0)
                                                                           {}
sdl_stb_font_cache::sdl_stb_font_cache ()
  : sttfont_font_cache (), mRenderer (NULL)
                                                                      {}
sdl_stb_font_cache::~ sdl_stb_font_cache ()
                               {
		clearGlyphs();
		}
void sdl_stb_font_cache::clearGlyphs ()
                            {
		for (auto & g : mGlyphs) {
			if (g.second.mSdlTexture) {
				SDL_DestroyTexture(g.second.mSdlTexture);
				g.second.mSdlTexture = NULL;
				}
			if (g.second.mSdlSurface) {
				SDL_FreeSurface(g.second.mSdlSurface);
				g.second.mSdlSurface = NULL;
				}
			}
		mGlyphs.clear();
		}
void sdl_stb_font_cache::bindRenderer (SDL_Renderer * _mRenderer)
                                                     {
		mRenderer = _mRenderer;
		}
void sdl_stb_font_cache::genGlyph_writeData (uint32_t const codepoint, sttfont_glyph * gOut, unsigned char * bitmap2, int w, int h)
                                                                                                                        {
		sdl_stb_glyph* gOut2 = (sdl_stb_glyph*) gOut;
		gOut2->mSdlSurface = SDL_CreateRGBSurfaceFrom(bitmap2, w, h, 32, 4*w, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		gOut2->mSdlTexture = SDL_CreateTextureFromSurface(mRenderer, gOut2->mSdlSurface);
		}
sttfont_glyph * sdl_stb_font_cache::getGlyph (uint64_t const target)
                                                        {
		auto it = mGlyphs.find(target);
		if (it == mGlyphs.end())
			return NULL;
		return &((*it).second);
		}
sttfont_glyph * sdl_stb_font_cache::genGlyph_createAndInsert (uint64_t const target, uint32_t const codepoint, uint8_t const format)
                                                                                                                        {
		sdl_stb_glyph g;
		genGlyph(codepoint, format, &g);
		mGlyphs[target] = g;
		return getGlyph(target);
		}
void sdl_stb_font_cache::processCodepoint (int & x, int & y, uint32_t const codepoint, sttfont_format const * const format, bool isDrawing, int kerningAdv, int & overdraw)
                                                                                                                                                                {
		// Draws the character, advances x & y to the next position
		// NOTE: KErning
		uint8_t formatCode = 0;
		if (format)
			formatCode = format->format;
			
		sdl_stb_glyph * G = (sdl_stb_glyph*) getGlyphOrTofu(codepoint, formatCode);
		if (!G) {
			x += faceSize/2;
			return;
			}
		if (isDrawing) {
			SDL_Rect r;
			r.x = x + G->xOffset;
			r.y = y + G->yOffset + baseline;
			r.w = G->width;
			r.h = G->height;
			
			if (G->mSdlTexture) {
				if (format) {
					int charAdv = kerningAdv + G->xOffset;
					bool isColoured = (format->r < 255) || (format->g < 255) || (format->b < 255);
					uint8_t cr,cg,cb,ca;
					if (isColoured || formatCode) {
						SDL_SetTextureColorMod(G->mSdlTexture, format->r, format->g, format->b);
						SDL_GetRenderDrawColor(mRenderer, &cr,&cg,&cb,&ca);
						SDL_SetRenderDrawColor(mRenderer, format->r, format->g, format->b, 0);
						// Remove bleeding pixels
						SDL_Rect r2;
						r2.x = r.x; r2.y = r.y;
						r2.w = r.w; r2.h = r.h;
						if (r2.x < overdraw) {
							int dx = overdraw - r2.x;
							r2.x += dx;
							r2.w -= dx;
							}
						overdraw = r.x + r.w;
						SDL_RenderFillRect(mRenderer, &r2); //TODO: prevent overlapping!
						}
					if (formatCode)
						SDL_SetRenderDrawColor(mRenderer, format->r, format->g, format->b, 255);
					SDL_RenderCopy(mRenderer, G->mSdlTexture, NULL, &r);
					
					if (formatCode & sttfont_format::FORMAT_STRIKETHROUGH) {
						SDL_Rect r2;
						r2.w = G->width+strikethroughThickness + charAdv; r2.h = strikethroughThickness;
						if (r2.h < 1) r2.h = 1;
						r2.x = r.x-strikethroughThickness/2 - charAdv; r2.y = y + strikethroughPosition;
						SDL_RenderFillRect (mRenderer, &r2);
						}
					if (formatCode & sttfont_format::FORMAT_UNDERLINE) {
						SDL_Rect r2;
						r2.w = G->width+underlineThickness + charAdv; r2.h = underlineThickness;
						if (r2.h < 1) r2.h = 1;
						r2.x = r.x-underlineThickness/2 - charAdv; r2.y = y + underlinePosition;
						SDL_RenderFillRect (mRenderer, &r2);
						}
					
					if (isColoured || formatCode) {
						SDL_SetRenderDrawColor(mRenderer, cr, cg, cb, ca);
						SDL_SetTextureColorMod(G->mSdlTexture, 255,255,255);
						}
					}
				else {
					overdraw = SSF_INT_MIN;
					SDL_RenderCopy(mRenderer, G->mSdlTexture, NULL, &r);
					}
				}
			}
		x += scale*G->advance;
		}
SDL_Texture * sdl_stb_font_cache::renderTextToTexture (char const * c, uint32_t const maxLen, int * widthOut, int * heightOut)
                                                                                                                                      {
		return renderTextToTexture_worker(NULL, c, maxLen, widthOut, heightOut);
		}
SDL_Texture * sdl_stb_font_cache::renderTextToTexture (sttfont_formatted_text const & formatted, int * widthOut, int * heightOut)
                                                                                                                                    {
		return renderTextToTexture_worker(&formatted, NULL, -1, widthOut, heightOut);
		}
SDL_Texture * sdl_stb_font_cache::renderTextToTexture_worker (sttfont_formatted_text const * formatted, char const * c, uint32_t const maxLen, int * widthOut, int * heightOut)
                                                                                                                                                                                       {
		int width, height;
		if (formatted)
			getTextSize(width, height, *formatted);
		else
			getTextSize(width, height, c, maxLen);
		
		SDL_Texture * RT = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET, width, height);
		SDL_Texture * oldRT = SDL_GetRenderTarget(mRenderer);
		const bool isClipping = SDL_RenderIsClipEnabled(mRenderer);
		SDL_Rect oldScissor;
		if (isClipping) SDL_RenderGetClipRect(mRenderer, &oldScissor);
		SDL_SetRenderTarget(mRenderer, RT);
		SDL_SetTextureBlendMode(RT, SDL_BLENDMODE_NONE);
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 0); // Must be the same colour as the text
		// Fill RT with blank pixels. SDL_RenderClear will leave artefacts in SDL Software mode
		SDL_Rect r;
		r.x = 0; r.y = 0;
		r.w = width; r.h = height;
		SDL_RenderFillRect (mRenderer, &r); // Must be rendered with a fill rect
		
		SDL_SetTextureBlendMode(RT, SDL_BLENDMODE_BLEND);
		if (formatted)
			drawText(0, 0, *formatted);
		else
			drawText(0, 0, c, maxLen);
		
		SDL_SetRenderTarget(mRenderer, oldRT);
		if (isClipping) SDL_RenderSetClipRect(mRenderer, &oldScissor);
		
		*widthOut = width;
		*heightOut = height;
		return RT;
		}
SDL_Texture * sdl_stb_font_cache::renderTextToTexture (SSF_STRING const & str, int * widthOut, int * heightOut)
                                                                                                                  {
		return renderTextToTexture(str.data(), str.length(), widthOut, heightOut);
		}
void sdl_stb_font_cache::renderTextToObject (sttfont_prerendered_text * textOut, char const * c, uint32_t const maxLen)
                                                                                                                 {
		sdl_stb_prerendered_text * textOut2 = (sdl_stb_prerendered_text*) textOut;
		textOut2->mSdlTexture = renderTextToTexture(c, maxLen, &(textOut->width), &(textOut->height));
		}
void sdl_stb_font_cache::renderTextToObject (sttfont_prerendered_text * textOut, SSF_STRING const & str)
                                                                                             {
		sdl_stb_prerendered_text * textOut2 = (sdl_stb_prerendered_text*) textOut;
		textOut2->mSdlTexture = renderTextToTexture(str, &(textOut->width), &(textOut->height));
		}
void sdl_stb_font_cache::renderTextToObject (sttfont_prerendered_text * textOut, sttfont_formatted_text const & str)
                                                                                                         {
		sdl_stb_prerendered_text * textOut2 = (sdl_stb_prerendered_text*) textOut;
		textOut2->mSdlTexture = renderTextToTexture(str, &(textOut->width), &(textOut->height));
		}
#undef LZZ_INLINE
