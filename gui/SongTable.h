#pragma once
#include "IGUIElement.h"

class SongTable : IGUIElement {
public:
	SongTable(SoundManager* soundMangerInstance);
	void Render() override;
private:
	ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable;
	float TEXT_BASE_HEIGHT = 20.f;
	ImVec2 outer_size;
};