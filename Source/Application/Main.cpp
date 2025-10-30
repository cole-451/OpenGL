
int main(int argc, char* argv[]) {
	neu::file::SetCurrentDirectory("Assets");
	LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

	// initialize engine
	LOG_INFO("initialize engine...");
	neu::GetEngine().Initialize();

	// initialize OpenGL stuff
	std::vector<neu::vec3> points{ { -0.5f, -0.5f, 0 }, { 0, 0.5f, 0 }, { 0.5f, -0.5f, 0 }, { 0.5f, 0.5f, 0 } };
	std::vector<neu::vec3> colors{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, {1,0,1} };
	std::vector<neu::vec2> texturecoords{ {0,0}, {0.5f, 1.0f}, {1,1} };

	auto scene = std::make_unique<neu::Scene>();
	scene->Load("scenes/scene01.json");


	struct Vertex {
		neu::vec3 position;
		neu::vec3 color;
		neu::vec2 texturecoords;
	};

	std::vector<Vertex> vertices{
		{ { -0.5f, -0.5f, 0 }, { 1, 0, 0 },{0,0} },
		{{ -0.5f, 0.5f, 0 }, { 0, 1, 0 },{0.0f, 1.0f} },
		{{ 0.5f, 0.5f, 0 },{ 0, 0, 1 },{1,1} },
		{{ 0.5f, -0.5f, 0 },{ 0, 0, 1 },{1,0} }
	};

	std::vector<GLshort> indices{ 0, 1, 2, 2, 3, 0 };

	//vertex buffer
	neu::res_t<neu::VertexBuffer> vb = std::make_shared<neu::VertexBuffer>();
	vb->CreateVertexBuffer((GLsizei)sizeof(Vertex)* vertices.size(), (GLsizei)vertices.size(), vertices.data());
	vb->CreateIndexBuffer(GL_UNSIGNED_SHORT, (GLsizei)indices.size(), indices.data());

	vb->SetAttribute(0, 3, sizeof(Vertex), offsetof(Vertex, position));
	vb->SetAttribute(1, 3, sizeof(Vertex), offsetof(Vertex, color));
	vb->SetAttribute(2, 2, sizeof(Vertex), offsetof(Vertex, texturecoords));
	




	SDL_Event e;
	bool quit = false;

	// MAIN LOOP
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				quit = true;
			}
			ImGui_ImplSDL3_ProcessEvent(&e); // allows for us to move and scale the GUI
		}


		// update
		neu::GetEngine().Update();
		float dt = neu::GetEngine().GetTime().GetDeltaTime();
		if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

		scene->Update(dt);
		
		/*float speed = 10.0f;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x -= speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x += speed * dt;

		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.y += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.y -= speed * dt;

		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_Q)) camera.position.z += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_E)) camera.position.z -= speed * dt;*/


		




		//USE THIS FOR TRANSFORMATION.
		neu::vec2 mousepos = neu::GetEngine().GetInput().GetMousePosition();
		neu::vec2 position;
		position.x = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetWidth(), -1.0f, 1.0f, mousepos.x);
		position.y = -neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetHeight(), -1.0f, 1.0f, mousepos.y);


		float angle = neu::GetEngine().GetTime().GetTime() * 90.0f;
		neu::vec2 scale;
		scale.x = neu::math::sin(neu::GetEngine().GetTime().GetTime());
		scale.y = neu::math::sin(neu::GetEngine().GetTime().GetTime());

		// draw
		neu::GetEngine().GetRenderer().Clear();
		// start new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		// set ImGui
		
		
		ImGui::End();


		scene->Draw(neu::GetEngine().GetRenderer());
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



		neu::GetEngine().GetRenderer().Present();

	}

	neu::GetEngine().Shutdown();

	return 0;
}
