/************************************************************************************
This source file is part of the Theora Video Playback Library
For latest info, see http://libtheoraplayer.sourceforge.net/
*************************************************************************************
Copyright (c) 2008-2013 Kresimir Spes (kspes@cateia.com)
This program is free software; you can redistribute it and/or modify it under
the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
*************************************************************************************/
#include "TheoraPixelTransform.h"
#include "TheoraVideoClip.h"
#include "TheoraVideoFrame.h"

extern "C"
{
void decodeRGB  (struct TheoraPixelTransform* t);
void decodeRGBA (struct TheoraPixelTransform* t);
void decodeRGBX (struct TheoraPixelTransform* t);
void decodeARGB (struct TheoraPixelTransform* t);
void decodeXRGB (struct TheoraPixelTransform* t);
void decodeBGR  (struct TheoraPixelTransform* t);
void decodeBGRA (struct TheoraPixelTransform* t);
void decodeBGRX (struct TheoraPixelTransform* t);
void decodeABGR (struct TheoraPixelTransform* t);
void decodeXBGR (struct TheoraPixelTransform* t);
void decodeGrey (struct TheoraPixelTransform* t);
void decodeGrey3(struct TheoraPixelTransform* t);
void decodeGreyA(struct TheoraPixelTransform* t);
void decodeGreyX(struct TheoraPixelTransform* t);
void decodeAGrey(struct TheoraPixelTransform* t);
void decodeXGrey(struct TheoraPixelTransform* t);
void decodeYUV  (struct TheoraPixelTransform* t);
void decodeYUVA (struct TheoraPixelTransform* t);
void decodeYUVX (struct TheoraPixelTransform* t);
void decodeAYUV (struct TheoraPixelTransform* t);
void decodeXYUV (struct TheoraPixelTransform* t);
}

static void (*conversion_functions[])(struct TheoraPixelTransform*) = {0,
	decodeRGB,
	decodeRGBA,
	decodeRGBX,
	decodeARGB,
	decodeXRGB,
	decodeBGR,
	decodeBGRA,
	decodeBGRX,
	decodeABGR,
	decodeXBGR,
	decodeGrey,
	decodeGrey3,
	decodeGreyA,
	decodeGreyX,
	decodeAGrey,
	decodeXGrey,
	decodeYUV,
	decodeYUVA,
	decodeYUVX,
	decodeAYUV,
	decodeXYUV
};

TheoraVideoFrame::TheoraVideoFrame(TheoraVideoClip* parent)
{
	mReady = mInUse = false;
	mParent = parent;
	mIteration = 0;
	// number of bytes based on output mode
	int bytemap[]={0, 3, 4, 4, 4, 4, 3, 4, 4, 4, 4, 1, 3, 4, 4, 4, 4, 3, 4, 4, 4, 4};
	unsigned int size = mParent->getStride() * mParent->mHeight * bytemap[mParent->getOutputMode()];
	mBuffer = new unsigned char[size];
	memset(mBuffer, 255, size);
}

TheoraVideoFrame::~TheoraVideoFrame()
{
	if (mBuffer) delete [] mBuffer;
}

int TheoraVideoFrame::getWidth()
{
	return mParent->getWidth();
}

int TheoraVideoFrame::getStride()
{
	return mParent->mStride;
}

int TheoraVideoFrame::getHeight()
{
	return mParent->getHeight();
}

unsigned char* TheoraVideoFrame::getBuffer()
{
	return mBuffer;
}

void TheoraVideoFrame::decode(struct TheoraPixelTransform* t)
{
	if (t->raw != NULL)
	{
		memcpy(mBuffer, t->raw, t->rawStride * mParent->getHeight());
	}
	else
	{
		t->out = mBuffer;
		t->w = mParent->getWidth();
		t->h = mParent->getHeight();
		conversion_functions[mParent->getOutputMode()](t);
	}
	mReady = true;
}

void TheoraVideoFrame::clear()
{
	mInUse = mReady = false;
}
