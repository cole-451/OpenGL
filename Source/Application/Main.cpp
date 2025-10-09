
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene
    std::vector<neu::vec3> points{ { -0.5f, -0.5f, 0 }, { 0, 0.5f, 0 }, { 0.5f, -0.5f, 0 }, { 0.5f, 0.5f, 0 } };
    std::vector<neu::vec3> colors{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, {1,0,1} };


    //vertex buffer stuff.
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * points.size(), points.data(), GL_STATIC_DRAW);

    //vertex array
    GLuint vao;
    glGenVertexArrays(1, &vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //vertex shaders
    std::string vs_source;
    neu::file::ReadTextFile("Shaders/basic.vert", vs_source);
    const char* vs_cstr = vs_source.c_str();

    GLuint vs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_cstr, NULL);
    glCompileShader(vs);

    //fragment shaders
    std::string fs_source;
    neu::file::ReadTextFile("Shaders/basic.frag", fs_source);
    const char* fs_cstr = fs_source.c_str();

    GLuint fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_cstr, NULL);
    glCompileShader(fs);






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

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;


         
        
        //USE THIS FOR TRANSFORMATION.
        neu::vec2 mousepos = neu::GetEngine().GetInput().GetMousePosition();
        neu::vec2 position;
        position.x = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetWidth(), -1.0f, 1.0f, mousepos.x);
        position.y = -neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetHeight(), -1.0f, 1.0f, mousepos.y);
        

        float angle = neu::GetEngine().GetTime().GetTime() * 90.0f;
        neu::vec2 scale;
        scale.x = neu::math::sin(neu::GetEngine().GetTime().GetTime());
        scale.y = neu::math::sin(neu::GetEngine().GetTime().GetTime());


        std::cout << neu::GetEngine().GetInput().GetMousePosition().x<<std::endl;
        std::cout << neu::GetEngine().GetInput().GetMousePosition().y<<std::endl;

        // draw
        neu::GetEngine().GetRenderer().Clear();

        glLoadIdentity();
        glPushMatrix();
        glTranslatef(position.x, position.y, 0);
        glRotatef(angle, 0, angle, 0);
        glScalef(scale.x, scale.y, 0);
        glBegin(GL_QUADS);


        for (int i = 0; i < points.size(); i++) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }

        glEnd();

        glPopMatrix();
        
        neu::GetEngine().GetRenderer().Present();

    }

    neu::GetEngine().Shutdown();

    return 0;
}
