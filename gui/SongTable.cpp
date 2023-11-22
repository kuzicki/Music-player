#include "SongTable.h"

SongTable::SongTable(SoundManager* soundMangerInstance) : IGUIElement(soundMangerInstance) { 
	outer_size = ImVec2(450.0f, TEXT_BASE_HEIGHT * 8);
}

void SongTable::Render() {
	if (ImGui::BeginTable("table_scrolly", 4, flags, outer_size))
	{
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Artist", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Format", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Duration", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		ImGuiListClipper clipper;
		int itemCount = _soundManager->GetSongsSize();
		clipper.Begin(itemCount);
		static bool selected[100] = {};
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				ImGui::TableNextRow();
				SongPlaylist*& playlist = _soundManager->getPlaylist();
				for (int column = 0; column < 4; column++)
				{
					ImGui::TableSetColumnIndex(column);

					if (column == 0) {
						ImGui::Selectable((*playlist)[row].Title(), &selected[row], ImGuiSelectableFlags_SpanAllColumns);
					}
					else if (column == 1) {
						ImGui::Selectable((*playlist)[row].Artist(), &selected[row], ImGuiSelectableFlags_SpanAllColumns);
					}
					else if (column == 2) {
						ImGui::Selectable((*playlist)[row].FileFormat(), &selected[row], ImGuiSelectableFlags_SpanAllColumns);
					}
					else {
						ImGui::Selectable((*playlist)[row].Duration(), &selected[row], ImGuiSelectableFlags_SpanAllColumns);
					}
				}
			}
		}
		ImGui::EndTable();
	}
}