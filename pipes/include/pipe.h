struct Node {
    int grid_x;
    int grid_y;
    int grid_z;
    vec3 coordinates;
    bool occupied = false;
    std::vector<int> neighbours;
};


struct Vertex {
    vec3 position = vec3(0.0f, 0.0f, 0.0f);
    vec3 normal = vec3(0.0f, 0.0f, 0.0f);
};


struct Pipe {
    std::vector<vec3> nodes;

    std::vector<Vertex> vertices;

    unsigned int length = 0;
    
    bool active   = false;
    bool complete = false;

    unsigned int current_node = 0;
    unsigned int previous_node = 0;

    vec3 previous_direction = vec3(0.0f, 0.0f, 0.0f); //both normalised
    vec3 current_direction = vec3(0.0f, 0.0f, 0.0f);
    
    vec4 color = vec4 (1.0f, 1.0f, 1.0f, 1.0f);

    unsigned int VAO = 0;
    unsigned int VBO = 0;
};

void get_basis(vec3 n, vec3* e1, vec3* e2) { //has to be a smarter way of doing this...
    if (n.x != 0.0f) {
	if (n.x > 0.0f) {
	    e1->y = 1.0f;
	    e2->z = 1.0f;
	}
	else {
	    e1->z = -1.0f;
	    e2->y = -1.0f;
	}
    }
    else if (n.y != 0.0f) {
	if (n.y > 0.0f) {
	    e1->z = 1.0f;
	    e2->x = 1.0f;
	}
	else {
	    e1->x = -1.0f;
	    e1->z = -1.0f;
	}
    }
    else {
	if (n.z > 0.0f){
	    e1->x = 1.0f;
	    e2->y = 1.0f;
	}
	else {
	    e1->y = -1.0f;
	    e2->x = -1.0f;
	}
    }
}

void mesh_from_node(Pipe* p, int num_segments, float radius) {
    int length = p->length;

    vec3 n; //normal to end-plane of pipe

    vec3 current = p->current_direction;
    vec3 previous = p->previous_direction;

    n = normalize(current + previous); //if pipe is of minimal length then previous is the zero vector anyway
    
    //if the pipe is minimum length then we supply two vertex rings for the starting pipe section
    if (length == 2) {
	vec3 e1 = vec3(0.0f, 0.0f, 0.0f);
	vec3 e2 = vec3(0.0f, 0.0f, 0.0f);

	get_basis(n, &e1, &e2);
	
	vec3 p0 = p->nodes.front();
	vec3 p1 = p->nodes.back();

	Vertex q0;
	Vertex q1;
	
	for (int i = 0; i <= num_segments; i++) {
	    q0.position = p0 + (float)cos(i * TAU / num_segments) * radius * e1 + (float)sin(i * TAU / num_segments) * radius * e2;
	    q1.position = p1 + (float)cos(i * TAU / num_segments) * radius * e1 + (float)sin(i * TAU / num_segments) * radius * e2;

	    q0.normal = normalize(q0.position - p0);
	    q1.normal = normalize(q1.position - p1);
	    
	    p->vertices.push_back(q0);
	    p->vertices.push_back(q1);

	    /*
	    printf("Direction is (%f, %f, %f)\n", current.x, current.y, current.z);
	    printf("Basis vectors:\n   e1: (%f, %f, %f)   e2: (%f, %f, %f)\n", e1.x, e1.y, e1.z, e2.x, e2.y, e2.z);
	     
	    printf("Vertex : %i; First: (%f, %f, %f)\n", i, q0.position.x, q0.position.y, q0.position.z);
	    printf("            Second: (%f, %f, %f)\n\n", q1.position.x, q1.position.y, q1.position.z);
	    */
	    
	}
    }
    else { //otherwise we calculate the next ring of vertices and recalculate the previous ring
	int index;
	int vertex_count = p->vertices.size();

	vec3 p0 = p->nodes.at(length - 3); //last node but one
	
	vec3 p1 = p->nodes.at(length - 2); //previous node
	
	vec3 p2 = p->nodes.at(length - 1); //new node

	for (int i = 0; i <= num_segments; i++) {
	    	    
	    index = vertex_count - 2 - 2* (num_segments - i); //recall that vertices are interleaved for triangle stripping!

	    Vertex* q0 = &p->vertices.at(index);
	    Vertex* q1 = &p->vertices.at(index + 1);
	    Vertex q2;

	    float t = dot(n, p1) - dot(n, q0->position);
	    t = t/(dot(n, previous));

	    q1->position = q0->position + (t * previous); //updates the value in p's vertices
	    q1->normal = normalize(q1->position - p1);   //@TODO: this normal might lead to weird interpolation -- be sure to come back and check

	    t = dot(current, p2) - dot(current, q1->position);
	    q2.position = q1->position + (t * current);
	    q2.normal = normalize(q2.position - p2);

	    p->vertices.push_back(*q1);
	    p->vertices.push_back(q2);
	}
    }
}


Node grid[GRID_SIZE];


void generate_neighbours(Node grid[], int n) {
    int delta = GRID_WIDTH * GRID_HEIGHT;
    if (n % GRID_WIDTH == 0) {		                  //left face======================
	if (n == 0) {
	    grid[n].neighbours.push_back(1);              //front left bottom corner
	    grid[n].neighbours.push_back(GRID_WIDTH);
	    grid[n].neighbours.push_back(delta);
	}
	else if (n == delta - GRID_WIDTH) {
	    grid[n].neighbours.push_back(n - GRID_WIDTH); //front left top corner
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n + delta);
	}
	else if (n == GRID_SIZE - delta) {                //back left bottom corner
	    grid[n].neighbours.push_back(n - delta);
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	}
	else if (n == GRID_SIZE - GRID_WIDTH) {           //back left top corner
	    grid[n].neighbours.push_back(n - delta);
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	}
	else if (n < delta) {                             //front left edge
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	    grid[n].neighbours.push_back(n + delta);
	}
	else if (n > (GRID_SIZE - delta)) {               //back left edge
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	    grid[n].neighbours.push_back(n - delta);
	}
	else if (n % delta == 0) {                        //bottom left edge
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n + delta);
	    grid[n].neighbours.push_back(n - delta);
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	}
	else if ((n + GRID_WIDTH) % delta == 0) {         //top left edge
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n + delta);
	    grid[n].neighbours.push_back(n - delta);
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	}
	else {                                           //remainder of left face
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	    grid[n].neighbours.push_back(n - delta);
	    grid[n].neighbours.push_back(n + delta);
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	}
    }
    else if ((n % GRID_WIDTH) == (GRID_WIDTH - 1)) {	  //right face=======================
	if (n == GRID_WIDTH - 1) {
	    grid[n].neighbours.push_back(n - 1);          //front right bottom corner
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	    grid[n].neighbours.push_back(n + delta);
	}
	else if (n == delta - 1) {                        //front right top corner
	    grid[n].neighbours.push_back(n - 1);
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	    grid[n].neighbours.push_back(n + delta);
	}
	else if (n == GRID_SIZE - delta - 1 + GRID_WIDTH) { //back right bottom corner
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	    grid[n].neighbours.push_back(n - 1);
	    grid[n].neighbours.push_back(n - delta);
	}
	else if (n == GRID_SIZE - 1) {                    //back right top corner
	    grid[n].neighbours.push_back(n - GRID_WIDTH);   
	    grid[n].neighbours.push_back(n - 1);
	    grid[n].neighbours.push_back(n - delta);
	}
	else if (n < delta){                              //front right edge
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	    grid[n].neighbours.push_back(n - 1);
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	    grid[n].neighbours.push_back(n + delta);
	}
	else if (n > (GRID_SIZE - delta)) {               //back right edge
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	    grid[n].neighbours.push_back(n - 1);
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	    grid[n].neighbours.push_back(n - delta);
	}
	else if (n % delta == (GRID_WIDTH - 1)) {         //bottom right edge
	    grid[n].neighbours.push_back(n - 1);
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	    grid[n].neighbours.push_back(n - delta);
	    grid[n].neighbours.push_back(n + delta);
	}
	else if ((n + 1) % delta == 0) {                  //top right edge
	    grid[n].neighbours.push_back(n - 1);
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	    grid[n].neighbours.push_back(n - delta);
	    grid[n].neighbours.push_back(n + delta);
	}
	else {                                           //remainder of right face
	    grid[n].neighbours.push_back(n - 1);
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	    grid[n].neighbours.push_back(n - delta);
	    grid[n].neighbours.push_back(n + delta);
	}
    }
    else if (n < delta) {                                //front face===================
	if (n < GRID_WIDTH) {                            //bottom front edge
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	    grid[n].neighbours.push_back(n + delta);
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n - 1);
	}
	else if (n > (delta - GRID_WIDTH)) {             //top front edge
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	    grid[n].neighbours.push_back(n + delta);
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n - 1);
	}
	else {                                          //remainder of front face
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	    grid[n].neighbours.push_back(n + delta);
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n - 1);
	}
    }
    else if (n > (GRID_SIZE - delta)) {                //back face======================
	if (n < (GRID_SIZE - delta + GRID_WIDTH - 1)) {//bottom back edge
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n - 1);
	    grid[n].neighbours.push_back(n - delta);
	}
	else if (n > (GRID_SIZE - GRID_WIDTH)){        //top back edge
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n - 1);
	    grid[n].neighbours.push_back(n - delta);
	}
	else {                                         //remainder of back face
	    grid[n].neighbours.push_back(n + GRID_WIDTH);
	    grid[n].neighbours.push_back(n - GRID_WIDTH);
	    grid[n].neighbours.push_back(n + 1);
	    grid[n].neighbours.push_back(n - 1);
	    grid[n].neighbours.push_back(n - delta);
	}
    }
    else if ((n % delta) < GRID_WIDTH) {               //bottom face of grid
	grid[n].neighbours.push_back(n + GRID_WIDTH);
	grid[n].neighbours.push_back(n + delta);
	grid[n].neighbours.push_back(n - delta);
	grid[n].neighbours.push_back(n + 1);
	grid[n].neighbours.push_back(n - 1);
    }
    else if (((n + GRID_WIDTH) % delta) > 0) {         //top face of grid
	grid[n].neighbours.push_back(n - GRID_WIDTH);
	grid[n].neighbours.push_back(n + delta);
	grid[n].neighbours.push_back(n - delta);
	grid[n].neighbours.push_back(n + 1);
	grid[n].neighbours.push_back(n - 1);
    }
    else {	  				       //internal points of grid
	grid[n].neighbours.push_back(n + GRID_WIDTH);
	grid[n].neighbours.push_back(n - GRID_WIDTH);
	grid[n].neighbours.push_back(n + delta);
	grid[n].neighbours.push_back(n - delta);
	grid[n].neighbours.push_back(n + 1);
	grid[n].neighbours.push_back(n - 1);
    }
}


void init_pipe(Pipe* p) {
    int q = dist_smallest(gen) % 5;
    p->color = Colors[Palettes[current_palette][q]];
    
    glGenVertexArrays(1, &(p->VAO));
    glGenBuffers(1, &(p->VBO));

    glBindVertexArray(p->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, p->VBO);

    //glBufferData(GL_ARRAY_BUFFER, MAX_PIPE_LENGTH * sizeof(vec3), NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, 2 * (MAX_PIPE_LENGTH - 1) * (NUM_SEGMENTS + 1) * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
    //glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec3)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Pipe pipe_set[MAX_PIPE_NUMBER];


void begin_pipe(Pipe* p) {
    bool found = false;
    
    int q = dist_big(gen);
    int tries = 1;
    
    while (!found) {
	if (tries > 20) {
	    found = true;
	    return;
	}
	if (grid[q].occupied) {
	    q = (q + 313) % GRID_SIZE;
	    tries += 1;
	    continue;
	}
	else {
	    for (int i = 0; i < grid[q].neighbours.size(); i++) {
		int modulus = grid[q].neighbours.size();
		int candidate = grid[q].neighbours[(i + q) % modulus];
		if (grid[candidate].occupied == false) {
		    found = true;
		    p->nodes.push_back(grid[candidate].coordinates);
		    p->nodes.push_back(grid[q].coordinates);
		    p->current_node = q;
		    p->previous_node = candidate;

		    vec3 direction = grid[q].coordinates - grid[candidate].coordinates;
		    p->current_direction = normalize(direction);
		    
		    p->length = 2;
		    
		    grid[q].occupied = true;
		    grid[candidate].occupied = true;

		    return;
		}
		else {continue;}
	    }
	}
	q = (q + 313) % GRID_SIZE;
	tries += 1;
    }
}


void draw_pipe(Pipe* p, Shader shader) {
    shader.use();
    
    glBindVertexArray(p->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, p->VBO);

    //set uniform pipe_color

    shader.setVec4("pipe_color", p->color);

    if (p->complete == true) {
	//glDrawArrays(GL_LINE_STRIP, 0, p->length);
	for (int i = 0; i < (p->length - 1); i++) {
	    glDrawArrays(GL_TRIANGLE_STRIP, 2 * i * (NUM_SEGMENTS + 1), 2 * (NUM_SEGMENTS + 1));
	}
    }
    else if (p->length == 2) {
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3), &(p->nodes.front()));
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3), sizeof(vec3), &(p->nodes.back()));
	//glDrawArrays(GL_LINE_STRIP, 0, 2);
	
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(Vertex) * (NUM_SEGMENTS + 1), &(p->vertices.front()));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (NUM_SEGMENTS + 1));
	
    }
    else {
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3) * (p->length - 1), sizeof(vec3), &(p->nodes.back()));
	//glDrawArrays(GL_LINE_STRIP, 0, p->length);

        int index_offset = 2 * (NUM_SEGMENTS + 1) * (p->length - 3);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(vec3) * index_offset, 8 * sizeof(vec3) * (NUM_SEGMENTS + 1), &(p->vertices.at(index_offset)));
	for (int i = 0; i < p->length - 1; i ++) {  
	    glDrawArrays(GL_TRIANGLE_STRIP, 2 * i * (NUM_SEGMENTS + 1), 2 * (NUM_SEGMENTS + 1));
	}
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void grow_pipe(Pipe* p) {
    bool found = false;
    
    unsigned int current  = p->current_node;
    unsigned int previous = p->previous_node;

    unsigned int next_in_current_direction;

    int q = dist_small(gen);
    int r = dist_small(gen);
    
    int difference = current - previous;
    bool straight_ahead = false;
    
    for (int i = 0; i < grid[current].neighbours.size(); i++) {
	int modulus = grid[current].neighbours.size();
	if ((int)(grid[current].neighbours[(i + q) % modulus] - current) == difference) {
	    next_in_current_direction = grid[current].neighbours[(i + q) % modulus];
	    if (grid[next_in_current_direction].occupied == false) {
		straight_ahead = true;
		break;
	    }
	}
    }
    
    
    if (q > 15 && straight_ahead){
	p->nodes.push_back(grid[next_in_current_direction].coordinates);
	p->previous_node = current;
	p->current_node = next_in_current_direction;

	//calculate current direction pipe is going
	vec3 direction = grid[next_in_current_direction].coordinates - grid[current].coordinates;
	p->previous_direction = p->current_direction;
	p->current_direction = normalize(direction);
	
	p->length ++;
	
	grid[next_in_current_direction].occupied = true;
	
	found = true;
	return;
    }
    else {
	for (int i = 0; i < grid[current].neighbours.size(); i++) {
	    unsigned int modulus = grid[current].neighbours.size();
	    unsigned int candidate = grid[current].neighbours[(i + q + r) % modulus];
	    if (grid[candidate].occupied == false){
		p->nodes.push_back(grid[candidate].coordinates);
		
		p->previous_node = current;
		p->current_node = candidate;
		
		vec3 direction = grid[candidate].coordinates - grid[current].coordinates;

		p->previous_direction = p->current_direction;
		p->current_direction = normalize(direction);

		p->length ++;

		grid[candidate].occupied = true;
		found = true;
		return;
	    }
	}
    }
    if (!found) {
	p->complete = true;
    }
    return;
}

void update_pipes () {
    for (int i = 0; i < MAX_PIPE_NUMBER; i++) {

	Pipe* p = &pipe_set[i];
	
	//the pipe is inactive
	if (p->active == false) {continue;}

        //the pipe is complete and there's room for more pipes
	else if (p->complete == true && i < MAX_PIPE_NUMBER - 1) {
	    pipe_set[i + 1].active = true;
	    continue;
	}

	//the pipe is empty and hasn't been attempted yet
	else if (p->complete == false && p->length == 0) {
	    begin_pipe(p);
	    mesh_from_node(p, NUM_SEGMENTS, PIPE_RADIUS);
	    return;
	}

        //the pipe is at max size
	else if (p->length == MAX_PIPE_LENGTH) {
	    p->complete = true;
	}

	//otherwise attempt to grow the pipe 
	else if (p->complete == false) {
	    grow_pipe(p);
	    mesh_from_node(p, NUM_SEGMENTS, PIPE_RADIUS);
	}
    }
}

void reset_state (mat4& model, mat4& projection, Shader shader) {
    //reset pipes
    for (int i = 0; i < MAX_PIPE_NUMBER; i++) {
	Pipe* p = &pipe_set[i];
	
	p->active = false;
	p->complete = false;
	p->current_node = 0;
	p->previous_node = 0;
	p->current_direction = vec3(0.0f, 0.0f, 0.0f);
	p->previous_direction = vec3(0.0f, 0.0f, 0.0f);
	p->nodes.clear();
	p->vertices.clear();
	p->length = 0;
	p->VAO = 0;
	p->VBO = 0;
	init_pipe(p);
    }

    //reset grid
    for (int n = 0; n < GRID_SIZE; n++) {
	grid[n].occupied = false;
    }

    //generate new viewing angle
    int q = dist_angle(gen);
    int r = (q + 17) % 15;
    current_palette = q % 6;
    model = rotate(model, radians((float)q), vec3(0.0f, 1.0f, 0.0f));
    projection = perspective(radians((float)(pow(-1, r)*r) + 45.0f), 1.0f, 0.1f, 100.f);
    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("projection", projection);
}


