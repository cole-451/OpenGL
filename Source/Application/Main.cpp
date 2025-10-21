
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
	

	auto model3d = std::make_shared<neu::Model>();
	model3d->Load("Models/Flynn.obj");


	//shaders
	auto vs = neu::Resources().Get<neu::Shader>("Shaders/basic.vert", GL_VERTEX_SHADER);
	auto fs = neu::Resources().Get<neu::Shader>("Shaders/basic.frag", GL_FRAGMENT_SHADER);


	auto program = std::make_shared<neu::Program>();
	program->AttachShader(vs);
	program->AttachShader(fs);
	program->Link();
	program->Use();



	float rotation = 0;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	
	//something wrong here...
	neu::res_t <neu::Texture> texture = neu::Resources().Get<neu::Texture>("Textures/beast.png");
	
	program->SetUniform("u_texture", 0);
	program->SetUniform("u_model", model);

	// now we need to make a connection to the uniform for the time variable we had
	program->SetUniform("u_time", neu::GetEngine().GetTime().GetTime());

	glm::vec3 evilEye{ 0,50,50 };




	SDL_Event e;
	bool quit = false;

	// MAIN LOOP
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				quit = true;
			}
		}

		// update
		neu::GetEngine().Update();
		float dt = neu::GetEngine().GetTime().GetDeltaTime();
		if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

		rotation += dt;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		program->SetUniform("u_model", model);

		//evilEye.x = neu::GetEngine().GetInput().GetMouseDelta().x;
		//evilEye.z = neu::GetEngine().GetInput().GetMouseDelta().y;

		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) evilEye.x -= 5 * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) evilEye.x += 5 * dt;

		glm::mat4 view = glm::lookAt(evilEye, evilEye + glm::vec3{0,0,-1},glm::vec3{0,1,0});
		
		program->SetUniform("u_view", view);

		float aspect = (float)neu::GetEngine().GetRenderer().GetWidth() / neu::GetEngine().GetRenderer().GetHeight();

		glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
		program->SetUniform("u_projection", projection);




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

		//vb->Draw(GL_TRIANGLES);

		model3d->Draw(GL_TRIANGLES);
		



		neu::GetEngine().GetRenderer().Present();

	}

	neu::GetEngine().Shutdown();

	return 0;
}
