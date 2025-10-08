
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene
    std::vector<neu::vec3> points;
    std::vector<neu::vec3> colors;

    neu::vec3 color1{ 1,0,1 };
    neu::vec3 color2{ 1,0,1 };
    neu::vec3 color3{ 1,0,1 };
    neu::vec3 color4{ 1,0,1 };
    colors.push_back(color1);
    colors.push_back(color2);
    colors.push_back(color3);
    colors.push_back(color4);

    neu::vec3 point1{ -1,1,0 };
    points.push_back(point1);
    neu::vec3 point2{ 1,0,0 };
    points.push_back(point2);
    neu::vec3 point3{ 1,1,0 };
    neu::vec3 point4{ 1, -1, 0 };
    points.push_back(point3);
    points.push_back(point4);



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
        neu::vec2 position = neu::GetEngine().GetInput().GetMousePosition();

        float angle = neu::GetEngine().GetTime().GetTime() * 90.0f;



        std::cout << neu::GetEngine().GetInput().GetMousePosition().x<<std::endl;
        std::cout << neu::GetEngine().GetInput().GetMousePosition().y<<std::endl;

        // draw
        neu::GetEngine().GetRenderer().Clear();

        glLoadIdentity();
        glPushMatrix();
        glTranslatef(position.x, position.y, 0);
       // glRotatef(angle, 1, 0, 0); make an angle variable
        //glScalef(scale, scale, 0);
        glBegin(GL_QUADS);

       /* for (neu::vec3& color: colors) {
            glColor3f(color.r, color.g, color.b);
        }
        for (neu::vec3& point : points) {
            glColor3f(point.x, point.y, point.z);
        }*/

        for (int i = 0; i < points.size(); i++) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }


        /*glColor3f(1,0,0);
        glVertex3f(-1,1,0);
        glColor3f(1,1,0);
        glVertex3f(1,0,0);
        glColor3f(1,1,0);
        glVertex3f(1,1,0);
        glColor3f(1, 0, 1);
        glVertex3f(1, -1, 0);*/

        glEnd();

        glPopMatrix();

        neu::GetEngine().GetRenderer().Present();

       /* neu::vec3 color{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();


        neu::GetEngine().GetRenderer().Present();*/
    }

    neu::GetEngine().Shutdown();

    return 0;
}
