#include "hex_collapse.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

void HexCollapseComponent::init() {
	std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
	auto &hex = app.getCurrentModel().getHexahedra();
	hex.connect();

	cell_highlights_attr.resize(hex.ncells(), 0.f);
	cell_highlights_attr.assign(hex.ncells(), 0.f);
	app.getCurrentModel().setHighlight(cell_highlights_attr);

	std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
	std::cout << "init in: " << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - begin2).count() << "ms" << std::endl;

	is_init = true;
}

void HexCollapseComponent::setup() {

	std::chrono::steady_clock::time_point bt = std::chrono::steady_clock::now();
	auto &hex = app.getCurrentModel().getHexahedra();
	// Compute hex layers
	DisjointSet ds(hex.ncells() * 24);

	for (auto h : hex.iter_halfedges()) {
		auto opp = h.opposite_f().opposite_c();
		if (opp.active())
			ds.merge(h, opp);
		
		ds.merge(h, h.next().next().opposite_f());
	}

	ds.get_sets_id(layers);

	std::chrono::steady_clock::time_point et = std::chrono::steady_clock::now();
	
	std::cout << 
		"setup in: " << 
		std::chrono::duration_cast<std::chrono::milliseconds>(et - bt).count() << 
		"ms" << 
	std::endl;

	is_setup = true;
}

void HexCollapseComponent::cleanup() {

}

bool HexCollapseComponent::draw_gui(ImGuiContext *ctx) {
	ImGui::SetCurrentContext(ctx);

	ImGui::Begin("Hex collapse tool");

	if (ImGui::Button("Select hex layer to collapse")) {
		init();
		setup();
	}

	ImGui::Checkbox("Auto smooth##chk_auto_smooth_collapse_tool", &auto_smooth);
	
	ImGui::End();

	return true;
}

void HexCollapseComponent::mouse_move(double x, double y) {
	auto &hex = app.getCurrentModel().getHexahedra();

	if (!hex.connected() || !st.edge.anyHovered() || !st.edge.hasChanged()) 
		return;

	// Remove last hovered cells
	for (auto c : last_hovered_cells) {
		if (cell_highlights_attr[c] >= 1.f && cell_highlights_attr[c] < 2.f)
			cell_highlights_attr[c] = 0.f;
	}

	hovered_cells.clear();
	for (auto h : hex.iter_halfedges()) {
		if (layers[h] == layers[st.edge.getHovered()])
			hovered_cells.push_back(h.cell());
	}

	last_hovered_cells = hovered_cells;

	// Set new hovered cells
	for (auto c : hovered_cells) {
		if (cell_highlights_attr[c] <= 0.f)
			cell_highlights_attr[c] = 1.f;
	}

	app.getCurrentModel().setHighlight(cell_highlights_attr);
}

void HexCollapseComponent::mouse_button(int button, int action, int mods) {

	if (!(is_init && is_setup) || button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS || !st.edge.anyHovered())
		return;

	// Remove last selected
	for (auto c : selected_cells) {
		if (cell_highlights_attr[c] >= 2.f)
			cell_highlights_attr[c] = 0.f;
	}

	selected_cells = hovered_cells;


	for (auto c : selected_cells) {
		cell_highlights_attr[c] = 2.f;
	}

	app.getCurrentModel().setHighlight(cell_highlights_attr);

	selected_layer = layers[st.edge.getHovered()];
}

void HexCollapseComponent::key_event(int key, int scancode, int action, int mods) {

	if (!(is_init && is_setup))
		return;

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		validate_callback();
	}
}

void HexCollapseComponent::validate_callback() {
	
	collapse();

	// if (auto_smooth)
	// 	BenjaminAPI::smooth(ctx.hex_bound->hex, *ctx.emb_attr, ctx.tet_bound->tri, *ctx.tri_chart);

	// Update rendering
	// Time elapsed
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	app.getCurrentModel().push();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "update in: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

	// Reinit
	init();
	setup();

	

	hovered_cells.clear();
	last_hovered_cells.clear();
	
}

void HexCollapseComponent::collapse(/*, CellFacetAttribute<int> &emb_attr*/) {
	
	auto &hex = app.getCurrentModel().getHexahedra();

	std::vector<int> layer;

	for (auto h : hex.iter_halfedges()) {
		if (layers[h] != selected_layer)
			continue;

		layer.push_back(h);

		auto nh = h.opposite_f().next().next();
		layer.push_back(nh);
		nh = nh.opposite_f().next().next();
		layer.push_back(nh);
		nh = nh.opposite_f().next().next();
		layer.push_back(nh);

	}


	std::vector<int> indirection_map(hex.nverts());
	std::vector<bool> cells_to_kill(hex.ncells(), false);
	{
		for (auto v : hex.iter_vertices())
			indirection_map[v] = v;


		bool is_forward_collapse = true;
		bool is_backward_collapse = true;
		for (auto h_idx : layer) {
			Volume::Halfedge h(hex, h_idx);
			
			for (auto ha : h.iter_CCW_around_edge()) {
				if (!ha.next().opposite_f().opposite_c().active()) {
					is_forward_collapse = false;
					break;
				}
				if (!ha.prev().opposite_f().opposite_c().active()) {
					is_backward_collapse = false;
					break;
				}
			}


			if (!is_forward_collapse && !is_backward_collapse) {
				// "Hard" to collapse
				return;
			}
		}

		for (auto h_idx : layer) {
			Volume::Halfedge h(hex, h_idx);
			if (is_backward_collapse)
				indirection_map[h.from()] = h.to();
			else 
				indirection_map[h.to()] = h.from();

			cells_to_kill[h.cell()] = true;
		}
	}


	for (int c = 0; c < hex.ncells(); c++) {
		for (int lv = 0; lv < 8; lv++) {
			hex.vert(c, lv) = indirection_map[hex.vert(c, lv)];
		}
	}

	// // propagate embedding
	// {
	// 	bool done = false;
	// 	while (!done) {
	// 		done = true;
	// 		for (auto f : hex.iter_facets()) {
	// 			// If cell is to kill and facet has an embedding (>= 0)
	// 			if (emb_attr[f] < 0 || !cells_to_kill[f.cell()]) 
	// 				continue;
				
	// 			// We move its embedding to the opposite interior facet
	// 			auto f_next = f.halfedge(0).opposite_f().next().next().opposite_f().facet().opposite();
				
	// 			if (f_next.active()) {
	// 				std::swap(emb_attr[f], emb_attr[f_next]);
	// 				done = false;
	// 			}
	// 		}
	// 	}
	// }



	// Remove degenerate hex
	hex.disconnect();
	hex.delete_cells(cells_to_kill);

	// Remove isolated vertices
	hex.delete_isolated_vertices();


}