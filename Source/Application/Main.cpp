
int main(int argc, char* argv[]) {
	neu::file::SetCurrentDirectory("Assets");
	LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

	// initialize engine
	LOG_INFO("initialize engine...");
	neu::GetEngine().Initialize();

	SDL_Event e;
	bool quit = false;

	

	//load scene
	auto scene = std::make_unique<neu::Scene>();
	scene->Load("scenes/scene01.json");


	

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
		

		float speed = 10.0f;
		

		// draw
		neu::GetEngine().GetRenderer().Clear();
		// start new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		// set ImGui
		
		ImGui::Begin("Editor");
		

		
		ImGui::End();


		scene->Draw(neu::GetEngine().GetRenderer());
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



		neu::GetEngine().GetRenderer().Present();

	}

	neu::GetEngine().Shutdown();

	return 0;
}
