enum Color {
    WHITE = 0,
    GREY,
    RED,
    BLUE,
    GREEN,
    YELLOW,
    CYAN,
    PEARL,
    TEAL,
    LAVENDER,
    BURGUNDY,
    ORANGE,
    GOLD,
    NAVY,
    MAROON
};

Color Palettes[6][5] = {
    {WHITE, CYAN, BLUE, NAVY, TEAL},
    {GREY, WHITE, RED, ORANGE, MAROON},
    {NAVY, RED, BURGUNDY, MAROON, RED},
    {PEARL, GOLD, ORANGE, GREY, TEAL},
    {GREEN, TEAL, GOLD, TEAL, GREEN},
    {WHITE, WHITE, CYAN, CYAN, ORANGE}
};

vec4 Colors[] = {
    vec4(1.0f, 1.0f, 1.0f, 1.0f),                          //WHITE    
    vec4(129.0/255.0f, 129.0/255.0f, 129.0/255.0f, 1.0f),  //GREY
    vec4(1.0f, 18.0/255.0f, 18.0/255.0f, 1.0f),            //RED
    vec4(93.0/255.0f, 102.0/255.0f, 219.0/255.0f, 1.0f),   //BLUE
    vec4(82.0/255.0f, 156.0/255.0f, 1.0f, 1.0f),           //GREEN
    vec4(236.0/255.0f, 212.0/255.0f, 36.0/255.0f, 1.0f),   //YELLOW
    vec4(30.0/255.0f, 228.0/255.0f, 228.0/255.0f, 1.0f),   //CYAN
    vec4(1.0f, 210.0/255.0f, 233.0/255.0f, 1.0f),          //PEARL
    vec4(88.0/255.0f, 168.0/255.0f, 155.0/255.0f, 1.0f),   //TEAL
    vec4(131.0/255.0f, 22.0/255.0f, 168.0/255.0f, 1.0f),   //LAVENDER
    vec4(186.0/255.0f, 0.0f, 60.0/255.0f, 1.0f),           //BURGUNDY
    vec4(1.0f, 132.0/255.0f, 0.0f, 1.0f),                  //ORANGE
    vec4(204.0/255.0f, 134.0/255.0f, 24.0/255.0f, 1.0f),   //GOLD
    vec4(16.0/255.0f, 10.0/255.0f, 129.0/255.0f, 1.0f),    //NAVY
    vec4(129.0/255.0f, 0.0f, 0.0f, 1.0f)                   //MAROON
};
