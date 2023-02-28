float cube_vertices[] = {
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,

    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,

    -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,

     1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,

    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,

    -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
};

void draw_cube(Shader shader) {
    shader.use();

    unsigned int VAO;
    unsigned int VBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}
