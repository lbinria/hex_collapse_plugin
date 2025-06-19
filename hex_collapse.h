// Salamesh core
#include <component.h>
#include <states.h>
// Ultimaille
#include <ultimaille/all.h>
// ImGui
#include "imgui.h"
#include "imgui_internal.h"
// Std
#include <iostream>

using namespace UM;

struct HexCollapseComponent : public Component {

	HexCollapseComponent(Hexahedra &hex/*, HexRenderer &hex_renderer*/, InputState &st) :
		hex(hex), 
		// hex_renderer(hex_renderer), 
		st(st) {}

	void init() final override {
		std::cout << "HexCollapseComponent initialized." << std::endl;
	}

	void setup() final override {
		// Setup code for HexCollapseComponent
	}

	void cleanup() final override {
		// Cleanup code for HexCollapseComponent
	}

	bool draw_gui(/* TODO here pass imgui context */ImGuiContext *ctx) final override {
		
		ImGui::SetCurrentContext(ctx);

		ImGui::Begin("Hex Collapse Component");

		if (ImGui::Button("Select hex layer to collapse")) {
			init();
			setup();
		}

		ImGui::Checkbox("Auto smooth##chk_auto_smooth_collapse_tool", &auto_smooth);
		ImGui::End();
		return true; // Return true if GUI was drawn
	}

	virtual void mouse_move(double x, double y) final override {
		// std::cout << "BLOP" << std::endl;
		// Handle mouse move events for HexCollapseComponent
	}

	virtual void mouse_button(int button, int action, int mods) final override {
		// Handle mouse button events for HexCollapseComponent
	}

	virtual void mouse_scroll(double xoffset, double yoffset) final override {
		// Handle mouse scroll events for HexCollapseComponent
	}

	virtual void key_event(int key, int scancode, int action, int mods) final override {
		// Handle key events for HexCollapseComponent
	}

	private:

	Hexahedra &hex;
	// HexRenderer &hex_renderer;
	InputState &st;

	bool is_init;
	bool is_setup;

	// // CellFacetAttribute<float> cell_highlights_attr;
	// std::vector<float> cell_highlights_attr;

	// std::vector<int> layers;
	// std::vector<int> last_hovered_cells;
	// std::vector<int> hovered_cells;
	// std::vector<int> selected_cells;
	// int selected_layer = -1;

	bool auto_smooth = false;

	// void collapse();
	
};