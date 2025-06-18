#include <component.h>
#include <iostream>
#include "imgui.h"
#include "imgui_internal.h"

struct HexCollapseComponent : public Component {

	void init() final override {
		std::cout << "HexCollapseComponent initialized." << std::endl;
	}

	void setup() final override {
		// Setup code for HexCollapseComponent
	}

	void cleanup() final override {
		// Cleanup code for HexCollapseComponent
	}

	bool draw_gui(/* TODO here pass imgui context */) final override {
		ImGui::Begin("Hex Collapse Component");
		// ImGui::Text("This is the Hex Collapse Component GUI.");
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
	
};