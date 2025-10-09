
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize OpenGL stuff
    std::vector<neu::vec3> points{ { -0.5f, -0.5f, 0 }, { 0, 0.5f, 0 }, { 0.5f, -0.5f, 0 }, { 0.5f, 0.5f, 0 } };
    std::vector<neu::vec3> colors{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, {1,0,1} };


    //vertex buffer stuff. We are telling our GPU to get some memory ready
    GLuint vbo[2];
    glGenBuffers(2, vbo);

    //position is vbo[0]
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * points.size(), points.data(), GL_STATIC_DRAW);

    //color is vbo[1]
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);

    //vertex array : contains a lot of the shader info.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //binding position
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //binding color
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //vertex shaders
    std::string vs_source;
    neu::file::ReadTextFile("Shaders/basic.vert", vs_source);
    const char* vs_cstr = vs_source.c_str();

    GLuint vs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_cstr, NULL);
    glCompileShader(vs);

    int success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(vs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

    //fragment shaders
    std::string fs_source;
    neu::file::ReadTextFile("Shaders/basic.frag", fs_source);
    const char* fs_cstr = fs_source.c_str();

    GLuint fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_cstr, NULL);
    glCompileShader(fs);
    
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(fs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

    //create program pipeline so both shaders are linked
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetProgramInfoLog(shaderProgram, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Program link failed: {}", infoLog);
    }

    //now we can use it!
    glUseProgram(shaderProgram);

    // now we need to make a connection to the uniform for the time variable we had
   GLint uniform = glGetUniformLocation(shaderProgram, "u_time");

   //make SURE that the uniform does not fail, or this will crash.
   ASSERT(uniform != -1);





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

        glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());


         
        
        //USE THIS FOR TRANSFORMATION.
        neu::vec2 mousepos = neu::GetEngine().GetInput().GetMousePosition();
        neu::vec2 position;
        position.x = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetWidth(), -1.0f, 1.0f, mousepos.x);
        position.y = -neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetHeight(), -1.0f, 1.0f, mousepos.y);
        

        float angle = neu::GetEngine().GetTime().GetTime() * 90.0f;
        neu::vec2 scale;
        scale.x = neu::math::sin(neu::GetEngine().GetTime().GetTime());
        scale.y = neu::math::sin(neu::GetEngine().GetTime().GetTime());


        /*std::cout << neu::GetEngine().GetInput().GetMousePosition().x<<std::endl;
        std::cout << neu::GetEngine().GetInput().GetMousePosition().y<<std::endl;*/

        // draw
        neu::GetEngine().GetRenderer().Clear();

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)points.size());

       /* glLoadIdentity();
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

        glPopMatrix();*/
        
        neu::GetEngine().GetRenderer().Present();

    }

    neu::GetEngine().Shutdown();

    return 0;
}
