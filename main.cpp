#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <cmath>
#include <chrono>
#include <ctime>

//#define EXTENT 5040
//#define STAGE 7

//#define EXTENT 362880
//#define STAGE  9

#define EXTENT 39916800
#define STAGE  11

unsigned int factorial(unsigned short int n) {
    unsigned int result = 1;
    for(int i = 1; i < n+1; i++) {
	result *= i;
    }
    return result;
}

struct row {
    bool visited = false;
    unsigned short int distance;
};

row table[EXTENT];

unsigned int index_from_row(int input[STAGE]) {
    unsigned int result = 0;

    for (int i=0; i < STAGE - 1; i++) {
	int count = 0;
	for (int j=i+1; j < STAGE; j++) {
	    if (input[j] < input[i]) {
		count += 1;
	    }
	}
	result += count * factorial(STAGE - i - 1);
    }
    return result;
}

void row_from_code(std::vector<int>& code, int (&output)[STAGE]){
    std::vector<int> rounds;
    for (int i = 0; i<STAGE; i++) {
	rounds.push_back(i+1);
    }

    for (int i = 0; i < STAGE; i++) {
	if (code.size() != STAGE) {
	    printf("ERROR: The given Lehmer code is invalid! Expected %d digits, got %d.\n", STAGE, code.size());
	    return;
	}
	if (code[i] > STAGE - 1 || code[i] < 0) {
	    printf("ERROR: The given Lehmer code is invalid! The element with index %d is out of range!\n", i);
	    return;
	}
	output[i] = rounds[code[i]];
	rounds.erase(rounds.begin() + code[i]);
    }
}

void row_from_index(int index, int (&output)[STAGE]) {
    if (index < 0 || index > EXTENT - 1) {
	printf("ERROR: The given index, %d, is outside the expected range!", index);
	return;
    }
    std::vector<int> code;
    for (int i = 0; i < STAGE - 1; i++) {
	int quotient = std::floor(index / factorial(STAGE - i - 1));
        code.push_back(quotient);
	int remainder = index - quotient * factorial(STAGE - i - 1);
	index = remainder;
    }
    code.push_back(0);
    row_from_code(code, output);
}




void pp(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[1], a[3], a[5], a[6], a[0], a[4], a[2]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[1], a[3], a[5], a[7], a[0], a[8], a[2], a[6], a[4]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[1], a[3], a[5], a[7], a[0], a[9], a[2], a[10], a[4], a[8], a[6]};

	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}

void inv_pp(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[4], a[0], a[6], a[1], a[5], a[2], a[3]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[4], a[0], a[6], a[1], a[8], a[2], a[7], a[3], a[5]};	
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[4], a[0], a[6], a[1], a[8], a[2], a[10], a[3], a[9], a[5], a[7]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}


void bb(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[1], a[3], a[4], a[2], a[0], a[5], a[6]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[1], a[3], a[5], a[6], a[0], a[4], a[2], a[7], a[8]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[1], a[3], a[5], a[7], a[0], a[8], a[2], a[6], a[4], a[9], a[10]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}

void inv_bb(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[4], a[0], a[3], a[1], a[2], a[5], a[6]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[4], a[0], a[6], a[1], a[5], a[2], a[3], a[7], a[8]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[4], a[0], a[6], a[1], a[8], a[2], a[7], a[3], a[5], a[9], a[10]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}


void bs(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[1], a[3], a[4], a[2], a[0], a[6], a[5]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[1], a[3], a[5], a[6], a[0], a[4], a[2], a[8], a[7]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[1], a[3], a[5], a[7], a[0], a[8], a[2], a[6], a[4], a[10], a[9]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}


void inv_bs(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[4], a[0], a[3], a[1], a[2], a[6], a[5]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[4], a[0], a[6], a[1], a[5], a[2], a[3], a[8], a[7]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[4], a[0], a[6], a[1], a[8], a[2], a[7], a[3], a[5], a[10], a[9]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }   
}


void pb(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[1], a[3], a[5], a[2], a[0], a[6], a[4]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[1], a[3], a[5], a[7], a[0], a[4], a[2], a[8], a[6]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[1], a[3], a[5], a[7], a[0], a[9], a[2], a[6], a[4], a[10], a[8]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}

void inv_pb(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[4], a[0], a[3], a[1], a[6], a[2], a[5]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[4], a[0], a[6], a[1], a[5], a[2], a[8], a[3], a[7]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[4], a[0], a[6], a[1], a[8], a[2], a[7], a[3], a[10], a[5], a[9]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}


void ps(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[1], a[3], a[5], a[2], a[0], a[4], a[6]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[1], a[3], a[5], a[7], a[0], a[4], a[2], a[6], a[8]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[1], a[3], a[5], a[7], a[0], a[9], a[2], a[6], a[4], a[8], a[10]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}

void inv_ps(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[4], a[0], a[3], a[1], a[5], a[2], a[6]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[4], a[0], a[6], a[1], a[5], a[2], a[7], a[3], a[8]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[4], a[0], a[6], a[1], a[8], a[2], a[7], a[3], a[9], a[5], a[10]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}


void bp(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[1], a[3], a[4], a[5], a[0], a[6], a[2]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[1], a[3], a[5], a[6], a[0], a[7], a[2], a[8], a[4]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[1], a[3], a[5], a[7], a[0], a[8], a[2], a[9], a[4], a[10], a[6]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}

void inv_bp(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[4], a[0], a[6], a[1], a[2], a[3], a[5]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[4], a[0], a[6], a[1], a[8], a[2], a[3], a[5], a[7]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[4], a[0], a[6], a[1], a[8], a[2], a[10], a[3], a[5], a[7], a[9]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}


void sp(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[1], a[3], a[4], a[6], a[0], a[5], a[2]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[1], a[3], a[5], a[6], a[0], a[8], a[2], a[7], a[4]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[1], a[3], a[5], a[7], a[0], a[8], a[2], a[10], a[4], a[9], a[6]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}

void inv_sp(int (&a)[STAGE]){
    if (STAGE == 7) {
	int b[7] = {a[4], a[0], a[6], a[1], a[2], a[5], a[3]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 9) {
	int b[9] = {a[4], a[0], a[6], a[1], a[8], a[2], a[3], a[7], a[5]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
    if (STAGE == 11) {
	int b[11] = {a[4], a[0], a[6], a[1], a[8], a[2], a[10], a[3], a[5], a[9], a[7]};
    	for (int i=0; i < STAGE; i++) {
	    a[i] = b[i];
	}
    }
}


void print_row (int perm[STAGE]) {
    for (int i = 0; i < STAGE; i++) {
	if (perm[i] < 10){
	    std::cout << perm[i];
	}
	else if (perm[i] == 10){
	    std::cout << 0;
	}
	else if (perm[i] == 11){
	    std::cout << "E";
	}
    }
}


void build_table() {
    std::cout << "Building new search table...\n";
    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds;

    int depth = 0;
    int current_index;
    int next_index;
    int current_row[STAGE];
    int next_row[STAGE];   //acts as an empty row for us to copy into
    std::list<int> queue;
    queue.push_back(0);
    table[0].visited = true;
    table[0].distance = 0;
    while (!queue.empty()) {
	current_index = queue.front();
	queue.pop_front();

	if (depth < table[current_index].distance) { //increase depth if all rows at a given distance have been exhausted
	    depth = table[current_index].distance;
	}
	
	row_from_index(current_index, current_row);
	
	for(int i = 0; i < STAGE; i++){
	    next_row[i] = current_row[i];
	}

	pp(next_row);
	next_index = index_from_row(next_row);
	if (!table[next_index].visited) {
	    table[next_index].visited = true;
	    table[next_index].distance = depth + 1;
	    queue.push_back(next_index);
	}

	for(int i = 0; i < STAGE; i++){
	    next_row[i] = current_row[i];
	}

	pb(next_row);
	next_index = index_from_row(next_row);
	if (!table[next_index].visited) {
	    table[next_index].visited = true;
	    table[next_index].distance = depth + 1;
	    queue.push_back(next_index);
	}
	
	for(int i = 0; i < STAGE; i++){
	    next_row[i] = current_row[i];
	}

	bp(next_row);
	next_index = index_from_row(next_row);
	if (!table[next_index].visited) {
	    table[next_index].visited = true;
	    table[next_index].distance = depth + 1;
	    queue.push_back(next_index);
	}

	for(int i = 0; i < STAGE; i++){
	    next_row[i] = current_row[i];
	}

	ps(next_row);
	next_index = index_from_row(next_row);
	if (!table[next_index].visited) {
	    table[next_index].visited = true;
	    table[next_index].distance = depth + 1;
	    queue.push_back(next_index);
	}
	
	for(int i = 0; i < STAGE; i++){
	    next_row[i] = current_row[i];
	}

	sp(next_row);
	next_index = index_from_row(next_row);
	if (!table[next_index].visited) {
	    table[next_index].visited = true;
	    table[next_index].distance = depth + 1;
	    queue.push_back(next_index);
	}
	
	for(int i = 0; i < STAGE; i++){
	    next_row[i] = current_row[i];
	}

	bs(next_row);
	next_index = index_from_row(next_row);
	if (!table[next_index].visited) {
	    table[next_index].visited = true;
	    table[next_index].distance = depth + 1;
	    queue.push_back(next_index);
	}
	
	for(int i = 0; i < STAGE; i++){
	    next_row[i] = current_row[i];
	}

	bb(next_row);
	next_index = index_from_row(next_row);
	if (!table[next_index].visited) {
	    table[next_index].visited = true;
	    table[next_index].distance = depth + 1;
	    queue.push_back(next_index);
	}
    }

    auto end = std::chrono::system_clock::now();
    elapsed_seconds = end - begin;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n\n";
    
}

int find_previous(int index, std::string &calling){
    int current_row[STAGE];
    row_from_index(index, current_row);
    int current_distance = table[index].distance;
    int prev_row[STAGE];                               //for copying
    int prev_index;

    for(int i = 0; i < STAGE; i++){
        prev_row[i] = current_row[i];
    }

    inv_pp(prev_row);
    prev_index = index_from_row(prev_row);

    if (table[prev_index].distance == table[index].distance - 1) {
	calling = "pp" + calling;
	return prev_index;
    }

    for(int i = 0; i < STAGE; i++){
        prev_row[i] = current_row[i];
    }

    inv_pb(prev_row);
    prev_index = index_from_row(prev_row);

    if (table[prev_index].distance == table[index].distance - 1) {
	calling = "pb" + calling;
	return prev_index;
    }
    
    for(int i = 0; i < STAGE; i++){
        prev_row[i] = current_row[i];
    }

    inv_ps(prev_row);
    prev_index = index_from_row(prev_row);

    if (table[prev_index].distance == table[index].distance - 1) {
	calling = "ps" + calling;
	return prev_index;

    }
    
    for(int i = 0; i < STAGE; i++){
        prev_row[i] = current_row[i];
    }

    inv_bp(prev_row);
    prev_index = index_from_row(prev_row);

    if (table[prev_index].distance == table[index].distance - 1) {
	calling = "bp" + calling;
	return prev_index;
    }

    for(int i = 0; i < STAGE; i++){
        prev_row[i] = current_row[i];
    }

    inv_sp(prev_row);
    prev_index = index_from_row(prev_row);

    if (table[prev_index].distance == table[index].distance - 1) {
	calling = "sp" + calling;
	return prev_index;
    }

    for(int i = 0; i < STAGE; i++){
        prev_row[i] = current_row[i];
    }

    inv_bb(prev_row);
    prev_index = index_from_row(prev_row);

    if (table[prev_index].distance == table[index].distance - 1) {
	calling = "bb" + calling;
	return prev_index;
    }
    
    for(int i = 0; i < STAGE; i++){
        prev_row[i] = current_row[i];
    }

    inv_bs(prev_row);
    prev_index = index_from_row(prev_row);

    if (table[prev_index].distance == table[index].distance - 1) {
	calling = "bs" + calling;
	return prev_index;
    }
}

void transpose_by(int (&a)[STAGE], int b[STAGE]) { //transpose row a by row b
    int copy[STAGE];
    for (int i = 0; i < STAGE; i++) {
	copy[i] = a[i];
    }
    for (int i = 0; i < STAGE; i++) {
	a[i] = copy[b[i] - 1];
    }
}

void get_transpose(int a[STAGE], int b[STAGE], int (&c)[STAGE]) { //c will be the transposition from a to b
    for (int i = 0; i < STAGE; i++) {
        for (int j = 0; j < STAGE; j++) {
	    if(b[i] == a[j]) {
		c[i] = j + 1;
	    }
	}
    }
}

void rotate_front(int (&row)[STAGE]) {
    int a = row[0];
    int b = row[1];
    int c = row[2];

    row[0] = b;
    row[1] = c;
    row[2] = a;
}

void swap_stroke(int (&row)[STAGE]) {
    int a = row[1];
    int b = row[2];

    row[1] = b;
    row[2] = a;

    int c;
    int d;

    for (int i = 3; i < STAGE; i+= 2) {
	c = row[i];
	d = row[i+1];

	row[i] = d;
	row[i+1] = c;
    }
}

void inv_p(int (&row)[STAGE]) {
    int a;
    int b;

    for (int i = 0; i < STAGE - 1; i+=2) {
	a = row[i];
	b = row[i + 1];

	row[i] = b;
	row[i + 1] = a;
    }
}

void inv_b(int (&row)[STAGE]) {
    int a;
    int b;

    for (int i = 0; i < STAGE - 3; i+=2) {
	a = row[i];
	b = row[i + 1];

	row[i] = b;
	row[i + 1] = a;
    }

    a = row[STAGE - 2];
    b = row[STAGE - 1];

    row[STAGE - 1] = a;
    row[STAGE - 2] = b;
}

void inv_s(int (&row)[STAGE]) {
    int a;
    int b;

    for (int i = 0; i < STAGE - 3; i+=2) {
	a = row[i];
	b = row[i + 1];

	row[i] = b;
	row[i + 1] = a;
    }
}

int optimise_end_row(int (&end_row)[STAGE], std::string &calling, bool slow_finish) {
    int index = index_from_row(end_row);

    std::string tail;
    
    for(int i=0; i < 3; i++) {
	rotate_front(end_row);
	int new_index = index_from_row(end_row);
	
	if (table[new_index].distance < table[index].distance && table[new_index].distance != 0)
	    {
		index = new_index;
	    }
	
    }
    
    swap_stroke(end_row);
    
    for(int i=0; i < 3; i++) {
	rotate_front(end_row);
       	int new_index = index_from_row(end_row);
	
	if (table[new_index].distance < table[index].distance && table[new_index].distance != 0)
	    {
		index = new_index;
	    }
    }
   
    swap_stroke(end_row);
    
    
    if (slow_finish) {

	for(int i=0; i < 3; i++) {
	    rotate_front(end_row);
	    inv_p(end_row);   //now check slow sixes before/after
	    int new_index = index_from_row(end_row);
	
	    if (table[new_index].distance < table[index].distance && table[new_index].distance != 0)
		{
		    index = new_index;
		    tail = "+p";
		}
	    inv_p(end_row);

	    if (i == 2) {rotate_front(end_row);}
	}

	swap_stroke(end_row);

    
	for(int i=0; i < 3; i++) {
	    rotate_front(end_row);
	    inv_p(end_row);
	    int new_index = index_from_row(end_row);

	    if (table[new_index].distance < table[index].distance && table[new_index].distance != 0)
		{
		    index = new_index;
		    tail = "+p";
		}
	    inv_p(end_row);

	    if (i == 2) {rotate_front(end_row);}
	}

	swap_stroke(end_row);

	for(int i=0; i < 3; i++) {
	    rotate_front(end_row);
	    inv_b(end_row);
	    int new_index = index_from_row(end_row);
	
	    if (table[new_index].distance < table[index].distance && table[new_index].distance != 0)
		{
		    index = new_index;
		    tail = "+b";
		}
	    inv_b(end_row);

	    if (i == 2) {rotate_front(end_row);}
	}

	swap_stroke(end_row);
	
	for(int i=0; i < 3; i++) {
	    rotate_front(end_row);
	    inv_b(end_row);
	    int new_index = index_from_row(end_row);
	
	    if (table[new_index].distance < table[index].distance && table[new_index].distance != 0)
		{
		    index = new_index;
		    tail = "+b";
		}
	    inv_b(end_row);

	    if (i == 2) {rotate_front(end_row);}
	}
    
	swap_stroke(end_row);

    
	for(int i=0; i < 3; i++) {
	    rotate_front(end_row);
	    inv_s(end_row);
	    int new_index = index_from_row(end_row);
	
	    if (table[new_index].distance < table[index].distance && table[new_index].distance != 0)
		{
		    index = new_index;
		    tail = "+s";
		}
	    inv_s(end_row);

	    if (i == 2) {rotate_front(end_row);}
	}
    
	swap_stroke(end_row);
    
	for(int i=0; i < 3; i++) {
	    rotate_front(end_row);
	    inv_s(end_row);
	    int new_index = index_from_row(end_row);
	
	    if (table[new_index].distance < table[index].distance && table[new_index].distance != 0)
		{
		    index = new_index;
		    tail = "+s";
		}
	    inv_s(end_row);

	    if (i == 2) {rotate_front(end_row);}
	}
    }

    calling = calling + tail;
    row_from_index(index, end_row);

    return index;  
}



void get_touch(int start_row[STAGE], int (&end_row)[STAGE], bool optimise, bool slow_finish) {
    std::cout << "Getting touch from search table...\n";
    
    int index;
    int transpose[STAGE];
    get_transpose(start_row, end_row, transpose);

    std::string calling;

    for (int i = 0; i < STAGE; i++) {
	end_row[i] = i + 1;
    }

    transpose_by(end_row, transpose);
	
    if (optimise){
	index = optimise_end_row(end_row, calling, slow_finish);
    }
    else {
	index = index_from_row(end_row);
    }

    int transposed_end[STAGE];
    
    for (int i = 0; i < STAGE; i++) {
	transpose[i] = end_row[i];
	transposed_end[i] = start_row[i];
    }

    transpose_by(transposed_end, end_row);
    
    int count = table[index].distance;
    int length = table[index].distance * 2;
    
    while (count != 0) {
	index = find_previous(index, calling);
	count -= 1;
    }
    
    if (calling.size() > length) { // in case of a slow six finish
        length += 1;
    }

    std::cout << "Touch length is " << length << " sixes\n\n"; 
    print_row(start_row);
    std::cout << " to ";
    print_row(transposed_end);
    std::cout << "\n";

    std::cout << "Calling: " + calling << "\n"; 
}

bool find_table (std::string fname) {
    std::cout << "Loading search table...\n";
    std::ifstream fin;
    char arr[4];

    fin.open(fname, std::ios::in);

    if (fin) {
	for (int i = 0; i < EXTENT; i++) {
	    fin.getline(arr, 4);
	    table[i].distance = std::stoi(arr);
	}
	fin.close();
	return true;
    } else {fin.close(); return false;}
}

bool export_table (std::string fname) {
    std::cout << "Attempting to write search table...\n";

    std::ofstream file(fname);
    std::stringstream data;

    for (int i = 0; i < EXTENT; i++) {
	data << table[i].distance << "\n";
    }

    if (file.is_open()) {
	file << data.rdbuf();
	file.close();
	return true;
    }
    else {return false;}
}



//=====================================================================================
//=====================================================================================


int main() {

    std::string fname = "table";
    fname += std::to_string(STAGE);
    fname += ".txt";
    
    if (!find_table(fname)) {
	std::cout << "No search table found on disk.\n";
	build_table();

	bool success = export_table(fname);

	if (!success) {
	    std::cout << "Error: could not write file " << fname << " to disk!\n";
	}
    }
    
    int start_row[STAGE]  = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int target_row[STAGE] = {6, 5, 4, 3, 2, 1, 7, 8, 9, 10, 11};
    bool optimise = true;
    bool slow_finish = true;

    get_touch(start_row, target_row, optimise, slow_finish);
    
    return 0; 
}


