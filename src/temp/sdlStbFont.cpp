// sdlStbFont.cpp
//

#include "sdlStbFont.hh"
#include <stdio.h>

#ifndef STB_TRUETYPE_INCLUDE_HANDLED
	////////////////////////////////////////
	// STB TRUETYPE 
	#define STB_TRUETYPE_IMPLEMENTATION
	#include <math.h>
	#include <string.h>
	#include <assert.h>
	#include <stdlib.h>
	#include "stb_truetype.h"
	////////////////////////////////////////
#endif

#include <SDL2/SDL.h>

#define LZZ_INLINE inline
void sdl_stb_format_callback::callbackOnDraw (sdl_stb_formatted_text const & text, int index, int x, int y, int xOffsetInitial, int xOffsetAfter, int segmentWidth, int segmentHeight)
                                                                                                                                                                             {}
uint8_t const sdl_stb_format::FORMAT_NONE = 0 << 0;
uint8_t const sdl_stb_format::FORMAT_BOLD = 1 << 0;
uint8_t const sdl_stb_format::FORMAT_ITALIC = 1 << 1;
uint8_t const sdl_stb_format::FORMAT_UNDERLINE = 1 << 2;
uint8_t const sdl_stb_format::FORMAT_STRIKETHROUGH = 1 << 3;
uint8_t const sdl_stb_format::FORMAT_RENDER_EVEN_IF_CALLBACK_EXISTS = 1 << 7;
sdl_stb_format::sdl_stb_format ()
  : r (255), g (255), b (255), a (255), format (0)
                                                                     {}
sdl_stb_format::sdl_stb_format (uint8_t const _format, uint8_t const _r, uint8_t const _g, uint8_t const _b, uint8_t const _a)
  : r (_r), g (_g), b (_b), a (_a), format (_format)
                                                                                      {}
void sdl_stb_format::combine (sdl_stb_format const & other)
                                                   {
		format |= other.format;
		r = 255*((r/255.0)*(other.r/255.0));
		g = 255*((g/255.0)*(other.g/255.0));
		b = 255*((b/255.0)*(other.b/255.0));
		a = 255*((a/255.0)*(other.a/255.0));
		}
sdl_stb_format sdl_stb_format::COLOR (uint8_t const _r, uint8_t const _g, uint8_t const _b, uint8_t const _a)
                                                                                                                  { return sdl_stb_format(_r,_g,_b,_a); }
sdl_stb_format sdl_stb_format::COLOUR (uint8_t const _r, uint8_t const _g, uint8_t const _b, uint8_t const _a)
                                                                                                                   { return sdl_stb_format(_r,_g,_b,_a); }
sdl_stb_format const sdl_stb_format::bold = sdl_stb_format(FORMAT_BOLD);
sdl_stb_format const sdl_stb_format::itallic = sdl_stb_format(FORMAT_ITALIC);
sdl_stb_format const sdl_stb_format::underline = sdl_stb_format(FORMAT_UNDERLINE);
sdl_stb_format const sdl_stb_format::strikethrough = sdl_stb_format(FORMAT_STRIKETHROUGH);
sdl_stb_format const sdl_stb_format::red = sdl_stb_format(FORMAT_NONE,255,  0,  0,255);
sdl_stb_format const sdl_stb_format::green = sdl_stb_format(FORMAT_NONE,  0,255,  0,255);
sdl_stb_format const sdl_stb_format::blue = sdl_stb_format(FORMAT_NONE,  0,  0,255,255);
sdl_stb_format const sdl_stb_format::yellow = sdl_stb_format(FORMAT_NONE,255,255,  0,255);
sdl_stb_format const sdl_stb_format::black = sdl_stb_format(FORMAT_NONE,  0,  0,  0,255);
sdl_stb_format const sdl_stb_format::white = sdl_stb_format(FORMAT_NONE,255,255,255,255);
sdl_stb_format const sdl_stb_format::magenta = sdl_stb_format(FORMAT_NONE,255,  0,255,255);
sdl_stb_format const sdl_stb_format::cyan = sdl_stb_format(FORMAT_NONE,  0,255,255,255);
sdl_stb_format const sdl_stb_format::grey = sdl_stb_format(FORMAT_NONE,128,128,128,255);
sdl_stb_format_reset const sdl_stb_format::reset;
sdl_stb_formated_text_item::sdl_stb_formated_text_item ()
  : callback (0)
                                                    {}
sdl_stb_formated_text_item::sdl_stb_formated_text_item (SSF_STRING const & _text, sdl_stb_format const & _format)
  : text (_text), format (_format), callback (0)
                                                                                                                                            {}
sdl_stb_formated_text_item::sdl_stb_formated_text_item (SSF_STRING_MS _text, sdl_stb_format const & _format)
  : text (_text), format (_format), callback (0)
                                                                                                                                                    {}
sdl_stb_formated_text_item & sdl_stb_formated_text_item::setCallback (sdl_stb_format_callback * _cb)
                                                                              { callback = _cb; return *this; }
sdl_stb_formatted_text::sdl_stb_formatted_text ()
                                {}
sdl_stb_formatted_text::sdl_stb_formatted_text (SSF_STRING const & text)
                                                        { *this << text; }
sdl_stb_formatted_text::sdl_stb_formatted_text (SSF_STRING_MS text)
                                                        { *this << text; }
sdl_stb_formatted_text::sdl_stb_formatted_text (char const * text)
                                                                { *this << text; }
sdl_stb_formatted_text & sdl_stb_formatted_text::operator << (SSF_STRING const & text)
                                                                      { mItems.push_back(sdl_stb_formated_text_item(text, activeFormat)); return *this; }
sdl_stb_formatted_text & sdl_stb_formatted_text::operator << (SSF_STRING_MS text)
                                                                 { mItems.push_back(sdl_stb_formated_text_item(text, activeFormat)); return *this;  }
sdl_stb_formatted_text & sdl_stb_formatted_text::operator << (char const * text)
                                                                { mItems.push_back(sdl_stb_formated_text_item(SSF_STRING(text), activeFormat)); return *this;  }
sdl_stb_formatted_text & sdl_stb_formatted_text::operator << (sdl_stb_format const & format)
                                                                            { activeFormat.combine(format); return *this; }
sdl_stb_formatted_text & sdl_stb_formatted_text::operator << (sdl_stb_format_reset const & reset)
                                                                                 { activeFormat = sdl_stb_format(); return *this; }
sdl_stb_formatted_text & sdl_stb_formatted_text::operator << (sdl_stb_formated_text_item const & obj)
                                                                                     { mItems.push_back(obj); return *this; }
sdl_stb_formatted_text & sdl_stb_formatted_text::operator << (sdl_stb_formated_text_item_MS obj)
                                                                                { mItems.push_back(obj); return *this; }
sdl_stb_prerendered_text::sdl_stb_prerendered_text ()
  : mSdlTexture (NULL), width (0), height (0)
                                                                             {}
void sdl_stb_prerendered_text::freeTexture ()
                            {
		if (mSdlTexture)
			SDL_DestroyTexture(mSdlTexture);
		mSdlTexture = NULL;
		}
void sdl_stb_prerendered_text::draw (SDL_Renderer * mRenderer, int const x, int const y)
                                                                       {
		SDL_Rect r;
		r.x = x;
		r.y = y;
		r.w = width;
		r.h = height;
		SDL_RenderCopy(mRenderer, mSdlTexture, NULL, &r);
		}
void sdl_stb_prerendered_text::drawWithColorMod (SDL_Renderer * mRenderer, int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a)
                                                                                                                                                            {
		SDL_SetTextureColorMod(mSdlTexture, r, g, b);
		if (a != 255)
			SDL_SetTextureAlphaMod(mSdlTexture, a);
		draw (mRenderer, x, y);
		}
void sdl_stb_memory::alloc (size_t const size)
                                      {
		data = SSF_NEW_ARR(char, size);
		ownsData = true;
		}
void sdl_stb_memory::transferTo (sdl_stb_memory & destination)
                                                      {
		destination.data = data;
		destination.ownsData = destination.data;
		ownsData = false;
		}
sdl_stb_memory::sdl_stb_memory ()
  : data (NULL), ownsData (false)
                                                        {}
sdl_stb_memory::~ sdl_stb_memory ()
                           {
		if (ownsData) {
			SSF_DEL_ARR(data);
			data = NULL;
			}
		}
sdl_stb_glyph::sdl_stb_glyph ()
  : mSdlTexture (0), mSdlSurface (0), advance (0), leftSideBearing (0), width (0), height (0), xOffset (0), yOffset (0)
                                                                                                                                       {}
sdl_stb_font_list::sdl_stb_font_list ()
  : format (0), next (NULL)
                                                    {}
sdl_stb_font_list::~ sdl_stb_font_list ()
                             {
		if (next) delete next;
		}
void sdl_stb_font_list::fetchFontForCodepoint (uint32_t const codepoint, uint8_t const format, stbtt_fontinfo * * mFontOut, int * indexOut)
                                                                                                                               {
		sdl_stb_font_list * working = this;
		
		*mFontOut = NULL;
		*indexOut = 0;
		
		while (working) {
			int index = stbtt_FindGlyphIndex(&(working->mFont), codepoint);
			if (index) {
				// Check for format in the variants
				if (format) {
					int bestBitsCount = 0;
					sdl_stb_font_list * bestMatch = NULL;
					sdl_stb_font_list * bestMatch2 = NULL;
					for (sdl_stb_font_list * f : mFormatedVariants) {
						uint8_t mask = (format & f->format);
						if (mask) {
							int nBits = 0;
							for (int i = 0; i < 7; ++i)
								if (mask & (1 << i)) nBits++;
							if (nBits > bestBitsCount) {
								bestBitsCount = nBits;
								bestMatch2 = bestMatch;
								bestMatch = f;
								}
							}
						}
					
					if (bestMatch) {
						int index2 = stbtt_FindGlyphIndex(&(bestMatch->mFont), codepoint);
						*mFontOut = &(working->mFont);
						*indexOut = index2;
						return;
						}
					if (bestMatch2) {
						int index2 = stbtt_FindGlyphIndex(&(bestMatch2->mFont), codepoint);
						*mFontOut = &(working->mFont);
						*indexOut = index2;
						return;
						}
					}
				
				// Format not found/no format
				*mFontOut = &(working->mFont);
				*indexOut = index;
				return;
				}
			working = working->next;
			}
		}
sdl_stb_font_cache::sdl_stb_font_cache ()
  : mRenderer (NULL), ascent (0), descent (0), lineGap (0), baseline (0), rowSize (0), scale (1.f), faceSize (20)
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
void sdl_stb_font_cache::setFaceSize (int const _faceSize)
                                              { faceSize = _faceSize; }
int sdl_stb_font_cache::getScaledRowSize () const
                                      { return scale * rowSize; }
void sdl_stb_font_cache::loadFont (char const * ttf_buffer, int index)
                                                               {
		stbtt_InitFont(&mFont.mFont, (const unsigned char *) ttf_buffer, stbtt_GetFontOffsetForIndex((const unsigned char *) ttf_buffer,index));
		stbtt_GetFontVMetrics(&mFont.mFont, &ascent, &descent, &lineGap);
		
		scale = stbtt_ScaleForPixelHeight(&mFont.mFont, faceSize);
		baseline = ascent*scale;
		rowSize = ascent - descent + lineGap;
		}
void sdl_stb_font_cache::loadFontManaged (sdl_stb_memory & memory, int index)
                                                                      {
		memory.transferTo(mFont.mMemory);
		loadFont(mFont.mMemory.data, index);
		}
void sdl_stb_font_cache::addFont (char const * ttf_buffer, int index)
                                                              {
		addFontWrap afw(ttf_buffer);
		afw.index = index;
		addFont_worker(afw, false);
		}
void sdl_stb_font_cache::addFontManaged (sdl_stb_memory & memory, int index)
                                                                     {
		addFontWrap afw(NULL);
		afw.memory = &memory;
		afw.index = index;
		addFont_worker(afw, false);
		}
void sdl_stb_font_cache::addFormatFont (uint8_t formatMask, char const * ttf_buffer, int index)
                                                                                        {
		addFontWrap afw(ttf_buffer);
		afw.index = index;
		addFont_worker(afw, true, formatMask);
		}
void sdl_stb_font_cache::addFormatFontManaged (uint8_t formatMask, sdl_stb_memory & memory, int index)
                                                                                               {
		addFontWrap afw(NULL);
		afw.memory = &memory;
		afw.index = index;
		addFont_worker(afw, true, formatMask);
		}
sdl_stb_font_cache::addFontWrap::addFontWrap (char const * c)
  : ttf_buffer (c), memory (NULL), index (0)
                                                                                     {}
void sdl_stb_font_cache::addFont_worker (addFontWrap & fwm, bool isFormatVariant, uint8_t formatMask)
                                                                                             { 
		sdl_stb_font_list * n = SSF_NEW(sdl_stb_font_list);
		sdl_stb_font_list * w = &mFont;
		while (w->next)
			w = w->next;
		n->format = formatMask;
		
		if (fwm.memory) {
			sdl_stb_memory & memory = *(fwm.memory);
			memory.transferTo(n->mMemory);
			stbtt_InitFont(&n->mFont, (const unsigned char *) n->mMemory.data, stbtt_GetFontOffsetForIndex((const unsigned char *) n->mMemory.data,fwm.index));
			}
		else {
			stbtt_InitFont(&n->mFont, (const unsigned char *) fwm.ttf_buffer, stbtt_GetFontOffsetForIndex((const unsigned char *) fwm.ttf_buffer,fwm.index));
			}
		if (isFormatVariant)
			w->mFormatedVariants.push_back(n);
		else
			w->next = n;
		}
void sdl_stb_font_cache::genGlyph (uint32_t const codepoint, uint8_t const format, sdl_stb_glyph * gOut)
                                                                                             {
		// Fetch font and index - existance check for glyph in font
		stbtt_fontinfo * mFontContaining;
		int mIndex;
		mFont.fetchFontForCodepoint(codepoint, format, &mFontContaining, &mIndex);
		
		if (!mIndex)
			return;
		
		// found the font! generate the glyph
	   	unsigned char *bitmap;
	   	int w,h,woff,hoff;
		bitmap = stbtt_GetCodepointBitmap(mFontContaining, 0, scale, codepoint, &w, &h, &woff, &hoff);
		
        // Convert bitmap to RGBA
        unsigned int sz = w*h;
        unsigned char bitmap2[sz*4];
        for (unsigned int i = 0; i < sz; ++i) {
        	bitmap2[i*4+0] = 255;
        	bitmap2[i*4+1] = 255;
        	bitmap2[i*4+2] = 255;
        	bitmap2[i*4+3] = bitmap[i];
        	}
        
        stbtt_FreeBitmap (bitmap, 0);
        
        if (w && h) {
			gOut->mSdlSurface = SDL_CreateRGBSurfaceFrom(bitmap2, w, h, 32, 4*w, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
			gOut->mSdlTexture = SDL_CreateTextureFromSurface(mRenderer, gOut->mSdlSurface);
			}
		
		gOut->width = w;
		gOut->height = h;
		stbtt_GetCodepointHMetrics(mFontContaining, codepoint, &gOut->advance, &gOut->leftSideBearing);
		
		gOut->xOffset = woff;
		gOut->yOffset = hoff;
		
		}
sdl_stb_glyph * sdl_stb_font_cache::getGlyph (uint64_t const target)
                                                        {
		auto it = mGlyphs.find(target);
		if (it == mGlyphs.end())
			return NULL;
		return &((*it).second);
		}
sdl_stb_glyph * sdl_stb_font_cache::getGenGlyph (uint32_t const codepoint, uint8_t const format)
                                                                                    {
		uint64_t target = codepoint | (uint64_t(format) << 32);
		sdl_stb_glyph * r = getGlyph(target);
		if (r) return r;
		sdl_stb_glyph g;
		genGlyph(codepoint, format, &g);
		mGlyphs[target] = g;
		return getGlyph(target);
		}
int sdl_stb_font_cache::getKerningAdvance (uint32_t const cp1, uint32_t const cp2)
                                                                      {
		return stbtt_GetCodepointKernAdvance(&mFont.mFont, cp1, cp2);
		}
int sdl_stb_font_cache::utf8_charsize (char const * c)
                                         {
		if (!c) return 0;
		if ((uint8_t)*c <= 0x7F) return 1;
		else if ((uint8_t)*c <= 0xE0) return 2;
		else if ((uint8_t)*c <= 0xF0) return 3;
		else
			return 4;
		}
uint32_t sdl_stb_font_cache::utf8_read (char const * c, uint32_t & seek, uint32_t const maxLen)
                                                                                  {
		if (!c) return 0;
		int chsz = utf8_charsize(c);
		seek += chsz;
	
		if (seek > maxLen) {
			return *c; //Buffer overflow - stop to be safe!
			}
			
		if (chsz == 1) return *c;		
		if (chsz == 2)
			return (((uint32_t((uint8_t) c[0] & 0b00111111) << 6)) | uint32_t((uint8_t) c[1] & 0b00111111));
		if (chsz == 3)
			return (uint32_t((uint8_t) c[0] & 0b00011111) << 12) | (uint32_t((uint8_t) c[1] & 0b00111111) << 6) | uint32_t((uint8_t) c[2] & 0b00111111);
		return (uint32_t((uint8_t) c[0] & 0b00001111) << 18) | (uint32_t((uint8_t) c[1] & 0b00111111) << 12) | (uint32_t((uint8_t) c[2] & 0b00111111) << 6) | uint32_t((uint8_t) c[3] & 0b00111111);
		}
int sdl_stb_font_cache::drawText (int const x, int const y, char const * c, uint32_t const maxLen)
                                                                                           {
		return processString(x, y, c, maxLen, NULL, true);
		}
int sdl_stb_font_cache::drawText (int const x, int const y, SSF_STRING const & str)
                                                                        {
		return drawText(x,y,str.data(),str.size());
		}
int sdl_stb_font_cache::drawText (int const x, int const y, int & widthOut, int & heightOut, char const * c, uint32_t const maxLen)
                                                                                                                            {
		return processString(x, y, c, maxLen, NULL, true, &widthOut, &heightOut);
		}
int sdl_stb_font_cache::drawText (int const x, int const y, SSF_STRING const & str, int & widthOut, int & heightOut)
                                                                                                         {
		return drawText(x,y,widthOut, heightOut, str.data(),str.size());
		}
int sdl_stb_font_cache::drawText (sdl_stb_formatted_text const & text, int x, int y)
                                                                        {
		int dummyWidth, dummyHeight;
		return drawText(text, x, y, dummyWidth, dummyHeight);
		}
int sdl_stb_font_cache::drawText (sdl_stb_formatted_text const & text, int x, int y, int & widthOut, int & heightOut)
                                                                                                         {
		int dummyWidth, dummyHeight;
		return processFormatted(text, x, y, true, &dummyWidth, &dummyHeight);
		}
int sdl_stb_font_cache::getTextSize (int & w, int & h, char const * c, uint32_t const maxLen)
                                                                                       {
		return processString(0, 0, c, maxLen, NULL, false, &w, &h);
		}
int sdl_stb_font_cache::getTextSize (int & w, int & h, SSF_STRING const & str)
                                                                   {
		return processString(0, 0, str.data(), str.size(), NULL, false, &w, &h);
		}
int sdl_stb_font_cache::getNumberOfRows (SSF_STRING const & str)
                                                     {
		int n = 1;
		for (const char c : str)
			if (c == '\n') n++;
		return n;
		}
int sdl_stb_font_cache::getTextHeight (SSF_STRING const & str)
                                                   {
		return scale*rowSize*getNumberOfRows(str);
		}
int sdl_stb_font_cache::processString (int const x, int const y, char const * c, uint32_t const maxLen, sdl_stb_format const * const format, bool const isDrawing, int * const widthOut, int * const heightOut, int const * const threshX, int const * const threshY, int * const caretPosition, int initialXOffset)
                                                                                                                                                                                                                                                                                                                                               {
		// Scan through function and extract the glyphs
		// returns the x position at the end
		uint32_t seek = 0;
		uint32_t uCharLast = 0;
		uint32_t uChar = utf8_read(c, seek, maxLen);
		int xx = x + initialXOffset;
		int yy = y;
		if (widthOut) *widthOut = 0;
		if (heightOut) *heightOut = 0;
		bool hasNewlined = false;
		
		const bool lookupCaret = caretPosition && threshX && threshY;
		if (lookupCaret) {
			*caretPosition = -1;
			}
		
		while (uChar && seek <= maxLen) {
			int xxl = xx;
			if (uChar == '\n') {
				if (widthOut)
					if (*widthOut < xx) *widthOut = xx;
				hasNewlined = true;
				xx = x;
				yy += scale*rowSize;
				}
			else {
				xx += scale*getKerningAdvance(uCharLast, uChar);
				processCodepoint(xx, yy, uChar, format, isDrawing);
				}
			
			if (lookupCaret) {
				//const int dx = xx - x;
				if (xx > *threshX && xxl <= *threshX ) {
					if (*threshX > xxl + (xx - xxl)/2)
						*caretPosition = seek; // right half of char
					else
						*caretPosition = seek-1; // left half of char
					return xx;
					}
				}
				
			uCharLast = uChar;
			uChar = utf8_read(c + seek, seek, maxLen);
			}
		if (widthOut) {
			if (*widthOut < xx) *widthOut = xx;
			*widthOut -= x;
			}
		if (heightOut) {
			if (*heightOut < yy) *heightOut = yy;
			*heightOut += scale*rowSize;
			*heightOut -= y;
			}
		return xx;
		}
int sdl_stb_font_cache::processFormatted (sdl_stb_formatted_text const & text, int x, int y, bool const isDrawing, int * const widthOut, int * const heightOut, int const * const threshX, int const * const threshY, int * const caretPosition, int initialXOffset)
                                                                                                                                                                                                                                                                                               {
		int xOffset = initialXOffset;
		
		if (widthOut) *widthOut = 0;
		if (heightOut) *heightOut = 0;
		
		for (unsigned int i = 0; i < text.mItems.size(); ++i) {
			const sdl_stb_formated_text_item & ssfti = text.mItems[i];
			
			bool isDrawingWorking = isDrawing;
			if (ssfti.callback && isDrawingWorking)
				isDrawingWorking = (ssfti.format.format & sdl_stb_format::FORMAT_RENDER_EVEN_IF_CALLBACK_EXISTS);
			
			int widthWorking, heightWorking;
			
			int xOffsetBefore = xOffset;
			xOffset = processString(x,y, ssfti.text.data(), ssfti.text.size(), &ssfti.format, isDrawingWorking, &widthWorking, &heightWorking, threshX, threshY, caretPosition, xOffset); // color!!!
			xOffset -= x;
			if (widthOut) *widthOut = *widthOut > widthWorking ? *widthOut : widthWorking;
			if (heightOut) *heightOut = *heightOut > heightWorking ? *heightOut : heightWorking;
			
			if (ssfti.callback)
				ssfti.callback->callbackOnDraw(text, i, x, y, xOffsetBefore, xOffset, widthWorking, heightWorking);
			}
		return xOffset + x;
		}
int sdl_stb_font_cache::getCaretPos (SSF_STRING const & str, int const relMouseX, int const relMouseY)
                                                                                          {
		int caretPosition = -1;
		processString(0,0, str.data(), str.length(), NULL, false, NULL, NULL, &relMouseX, &relMouseY, &caretPosition);
		return caretPosition;
		}
bool sdl_stb_font_cache::isTofu (sdl_stb_glyph * G)
                                        {
		if (!G) return true;
		if (!G->advance) return true; //unprintable characters have no width
		return false;
		}
sdl_stb_glyph * sdl_stb_font_cache::getGlyphOrTofu (uint32_t const codepoint, uint8_t const format)
                                                                                        {
		sdl_stb_glyph * G = getGenGlyph(codepoint, format);
		if (!isTofu(G)) return G;
		
		G = getGenGlyph((uint32_t) 0xFFFD, format); // https://en.wikipedia.org/wiki/Specials_(Unicode_block)#Replacement_character
		if (!isTofu(G)) return G;
			
		G = getGenGlyph((uint32_t) '?', format);
		if (!isTofu(G)) return G;
		
		if (format)
			return getGlyphOrTofu(codepoint, 0);
		
		return NULL;
		}
void sdl_stb_font_cache::processCodepoint (int & x, int & y, uint32_t const codepoint, sdl_stb_format const * const format, bool isDrawing)
                                                                                                                                {
		// Draws the character, advances x & y to the next position
		// NOTE: KErning
		uint8_t formatCode = 0;
		if (format)
			formatCode = format->format;
			
		sdl_stb_glyph * G = getGlyphOrTofu(codepoint, formatCode);
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
				if (format)
					SDL_SetRenderDrawColor(mRenderer, format->r, format->g, format->b, format->a);
				SDL_RenderCopy(mRenderer, G->mSdlTexture, NULL, &r);
				}
			}
		x += scale*G->advance;
		}
SDL_Texture * sdl_stb_font_cache::renderTextToTexture (char const * c, uint32_t const maxLen, int * widthOut, int * heightOut)
                                                                                                                                      {
		int width, height;
		getTextSize(width, height, c, maxLen);
		
		SDL_Texture * RT = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET, width, height);
		SDL_SetRenderTarget(mRenderer, RT);
		SDL_SetTextureBlendMode(RT, SDL_BLENDMODE_NONE);
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 0); // Must be the same colour as the text
		// Fill RT with blank pixels. SDL_RenderClear will leave artefacts in SDL Software mode
		SDL_Rect r;
		r.x = 0; r.y = 0;
		r.w = width; r.h = height;
		SDL_RenderFillRect (mRenderer, &r); // Must be rendered with a fill rect
		
		SDL_SetTextureBlendMode(RT, SDL_BLENDMODE_BLEND);
		drawText(0, 0, c, maxLen);
		SDL_SetRenderTarget(mRenderer, NULL);
		
		*widthOut = width;
		*heightOut = height;
		return RT;
		}
SDL_Texture * sdl_stb_font_cache::renderTextToTexture (SSF_STRING const & str, int * widthOut, int * heightOut)
                                                                                                                  {
		return renderTextToTexture(str.data(), str.length(), widthOut, heightOut);
		}
void sdl_stb_font_cache::renderTextToObject (sdl_stb_prerendered_text * textOut, char const * c, uint32_t const maxLen)
                                                                                                                 {
		textOut->mSdlTexture = renderTextToTexture(c, maxLen, &(textOut->width), &(textOut->height));
		}
void sdl_stb_font_cache::renderTextToObject (sdl_stb_prerendered_text * textOut, SSF_STRING const & str)
                                                                                             {
		textOut->mSdlTexture = renderTextToTexture(str, &(textOut->width), &(textOut->height));
		}
#undef LZZ_INLINE
