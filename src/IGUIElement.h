#pragma once
#include "SoundManager.h"
#include "../imgui/imgui.h"

class IGUIElement {
public:
	IGUIElement(SoundManager *soundManagerInstance) : _soundManager(soundManagerInstance) { }
	virtual void Render() = 0;
protected:
	SoundManager* _soundManager;
};