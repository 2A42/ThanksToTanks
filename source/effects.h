#pragma once

#include "common.h"

class Effect {
public:
	bool isOutOfFrames = false;

	Effect(Image& image, float Angle, float X, float Y, int W, int H, int FramesC, int FramesR, String Name);

	void update();

	Sprite& getSprite();

private:
	int x, y, w, h, angle, maxFramesC, maxFramesR;
	float duration, currentFrameC = 0, currentFrameR = 0;
	String name;
	Texture texture;
	Sprite sprite;
};