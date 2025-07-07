#pragma once

// Salamesh core
#include <app_interface.h>
#include <component.h>
#include <states.h>
#include <renderer.h>
// Ultimaille
#include <ultimaille/all.h>
// ImGui
#include "imgui.h"
#include "imgui_internal.h"
// Std
#include <iostream>

using namespace UM;

struct HexCollapseComponent : public Component {

	HexCollapseComponent(IApp &app) :
		app(app),
		st(app.getInputState())
		{}

	// Lifecycle functions
	void init() final override;
	void setup() final override;
	void cleanup() final override;
	bool draw_gui(ImGuiContext *ctx) final override;

	// Events functions
	virtual void mouse_move(double x, double y) final override;
	virtual void mouse_button(int button, int action, int mods) final override;
	virtual void mouse_scroll(double xoffset, double yoffset) final override {}
	virtual void key_event(int key, int scancode, int action, int mods) final override;
	void validate_callback(); // TODO map validate to a key, enable user to configure the key

	private:

	IApp &app;
	InputState &st;

	bool is_init;
	bool is_setup;

	std::vector<float> cell_highlights_attr;

	std::vector<int> layers;
	std::vector<int> last_hovered_cells;
	std::vector<int> hovered_cells;
	std::vector<int> selected_cells;
	int selected_layer = -1;

	bool auto_smooth = false;

	void collapse();
	
};