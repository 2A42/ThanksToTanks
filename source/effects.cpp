#include "effects.h"

Effect::Effect(Image& image, float Angle, float X, float Y, int W, int H, int FramesC, int FramesR, String Name)
{
	 angle = Angle - 90; x = X; y = Y; w = W; h = H; maxFramesC = FramesC; maxFramesR = FramesR; name = Name;
	 texture.loadFromImage(image);
	 sprite.setTexture(texture);
	 sprite.setOrigin(w / 2, h / 2);
	 sprite.setRotation(angle);
}

void Effect::update()
{
	sprite.setPosition(x, y);
	if (maxFramesR == 1) {
		if (name == "shell_trail") {
			sprite.setScale(1.5, 0.010);
			duration = 0.05;
		}

		else if (name == "muzzle_flash") {
			sprite.setScale(0.25, 0.25);
			duration = 0.15;
		}

		else if (name == "smoke") {
			duration = 0.1;
		}

		currentFrameC += duration;
		sprite.setTextureRect(IntRect(w * int(currentFrameC), 0, w, h));	
		if (int(currentFrameC) == maxFramesC) { currentFrameR++; }
	}

	else {
		if (name == "shell_exp") {
			sprite.setScale(0.25, 0.25);
			duration = 0.45;
		}

		else if (name == "tank_exp") {
			sprite.setScale(1.25, 1.25);
			duration = 0.35;
		}

		currentFrameC += duration;
		sprite.setTextureRect(IntRect(w * int(currentFrameC), h * int(currentFrameR), w, h));
		if (int(currentFrameC) == maxFramesC) { currentFrameC -= maxFramesC;  currentFrameR++; }
	}	

	if (int(currentFrameC * currentFrameR) == maxFramesC * maxFramesR) isOutOfFrames = true;
}

Sprite& Effect::getSprite() 
{	
	return sprite; 
}