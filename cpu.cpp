#include "cpu.h"

//initialize all variables from cpu.h
uint16_t opCode = 0;
uint16_t pc = 0;; //program counter
uint16_t I = 0;;
uint16_t stack[16] = {}; //16 levels to the stack
uint16_t sp = 0; //stack pointer to point to the stack position
uint8_t memory[4096] = {};  // 4k memory
uint8_t V[16] = {}; // has 16 registers (v0-vf)
uint8_t dT =0; //delay timer
uint8_t sT = 0; //sound timer
uint8_t graphics[64 * 32] = {}; //64 by 32 pixel display.
uint8_t key[16] = {}; //hex based keypad (0x0-0xF) 16 keys

uint8_t chip8Font[80] = { //the font
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void initCpu(){     //initialize cpu here
    pc = 0x200;     //pc starts at 0x200 check notes
    I = 0;          //reset index reg
    sp= 0;          //reset stack pointer
    opCode = 0;     //clear current opcode
    dT = 0;         //reset delay timer
    sT = 0;         //reset sound timer
    
    //clear display
    for(int i = 0; i < 64*32; i++){
        graphics[i] = 0;
    }
    //clear stack
    for(int i = 0; i<16; i++){
        stack[i] = 0;
    }
    //clear registers
    for(int i = 0; i < 16; i++){
        V[i] = 0;
    }
    //clear memory
    for(int i = 0; i < 4096; i++){
        memory[i] = 0;
    }
    //load font
    for(int i =0; i < 80; i++){
        memory[0x50 + i] = chip8Font[i];
    } //each character is 5 bytes and 5 * 16 = 80

}

/*Quick note about CPUs:

CPUS operate on a 3 step cycle: fetch, decode, execute.

Fetch: Read/get the next instruction from the memory
Decode: determine what instruction needs to be done
Execute: perform said instruction 
*/


//used for random number generator inside emulateCycle function

std::random_device rd1; //gets a seed for the random engine
std::mt19937 gen(rand()); // sets up random number generator
std::uniform_int_distribution<uint8_t> dist(0,255); //specifies range



void emulateCycle(){ //emulate 1 cycle of cpu here
    //fetch
    opCode = memory[pc] << 8 | memory[pc + 1]; //combines 2 bytes into 1 opcode (16 bits)
    pc+=2;
    //printf("PC = %03X, opcode = %04X\n", pc, opCode); //this is test terminal stuff

    //decode
    uint8_t inst = (opCode & 0xF000) >> 12; //gets the first nibble of the op code. ex: A of A023.
    switch(inst){
        case 0x0000:{ //case when first nibble is 0
            switch(opCode){
                case 0x00E0:{ //last nib is 0 ex: opcode 00E0
                    //CLS (clear the screen)
                    for(int i = 0; i < 64*32; i++){
                        graphics[i] = 0;
                    }
                    break;
                }
                case 0x00EE:{ //RET return from subroutine
                    sp--;
                    pc = stack[sp];
                    break;
                }

                default:{
                    //printf("Unknown opcode: 0x%04x\n", opCode);
                    break;
                }

            } 
            break;
        }
        case 0x0001:{
            //jumps to address NNN
            uint16_t address = opCode & 0x0FFF; //gets NNN from the opCode
            
            pc = address; //sets program counter to NNN.
            break;
        }
        case 0x0002:{
            //subroutine call to address NNN
            stack[sp] = pc; //store current pc mem in stack
            sp++; //add stack pointer
            pc = opCode & 0x0FFF; //put the program at that address so it can run the subroutine there
            break;
        }
        case 0x0003:{
            uint8_t regX = (opCode & 0x0F00) >> 8;  //register x
            uint16_t nn = opCode & 0x00FF; //value being checked

            if(V[regX] == nn){
                pc += 2; //skips to the next two byte instruction 
            }
            break;
        }
        case 0x0004:{
            uint8_t regX = (opCode & 0x0F00) >> 8;
            uint8_t nn = opCode & 0x00FF;

            if(V[regX] != nn){
                pc += 2;
            }
            break;
        }
        case 0x0005:{
            uint8_t regX = (opCode & 0x0F00) >> 8;
            uint8_t regY = (opCode & 0x00F0) >> 4;

            if(V[regX] == V[regY]){
                pc+=2;
            }
            break;
        }
        case 0x0006:{
            uint8_t regX = (opCode & 0x0F00) >> 8;
            uint8_t nn = (opCode & 0x00FF);

            V[regX] = nn;
            break;
        }
        case 0x0007:{
            uint8_t regX = (opCode & 0x0F00) >> 8;
            uint8_t nn = (opCode & 0x00FF);

            V[regX] = V[regX] + nn;
            break;
        }
        case 0x0008:{
            uint8_t lastNib = opCode & 0x000F;
            switch(lastNib){
                case 0x0000:{
                    uint8_t regX = (opCode & 0x0F00) >> 8;
                    uint8_t regY = (opCode & 0x00F0) >> 4;

                    V[regX] = V[regY];
                    break;
                }
                case 0x0001:{
                    uint8_t regX = (opCode & 0x0F00) >> 8;
                    uint8_t regY = (opCode & 0x00F0) >> 4;

                    V[regX] = V[regY] | V[regX];
                    break;
                }
                case 0x0002:{
                    uint8_t regX = (opCode & 0x0F00) >> 8;
                    uint8_t regY = (opCode & 0x00F0) >> 4;

                    V[regX] = V[regY] & V[regX];
                    break;
                }
                case 0x0003:{
                    uint8_t regX = (opCode & 0x0F00) >> 8;
                    uint8_t regY = (opCode & 0x00F0) >> 4;

                    V[regX] = V[regY] ^ V[regX];
                    break;
                }
                case 0x0004:{
                    uint8_t regX = (opCode & 0x0F00) >> 8;
                    uint8_t regY = (opCode & 0x00F0) >> 4;

                    uint16_t sum = V[regX] + V[regY];

                    if(sum > 255){
                        V[0xF] = 1;
                    }else{
                        V[0xF] = 0;
                    }
                    V[regX] = sum;
                    break;
                }
                case 0x0005:{
                    uint8_t regX = (opCode & 0x0F00) >> 8;
                    uint8_t regY = (opCode & 0x00F0) >> 4;

                    int diff = V[regX] - V[regY]; //not a uint because it has to be be signed. + or -

                    if(V[regX] >= V[regY]){ 
                        V[0xF] = 1;
                    }else{
                        V[0xF] = 0;
                    }
                    V[regX] = diff;
                    break;
                }
                case 0x0006:{
                    uint8_t regX = (opCode & 0x0F00) >> 8;

                    V[0xF] = V[regX] & 0x01; // if Vx has a 1 as the least, then V[F] changes to 1;
                    V[regX] = V[regX] >> 1;
                    break;
                }
                case 0x0007:{
                    uint8_t regX = (opCode & 0x0F00) >> 8;
                    uint8_t regY = (opCode & 0x00F0) >> 4;

                    if(V[regY] >= V[regX]) { 
                        V[0xF] = 1;
                    }
                    else{
                        V[0xF] = 0;
                    }
                    V[regX] = V[regY] - V[regX];
                    break;
                }
                case 0x000E:{
                    uint8_t regX = (opCode & 0x0F00) >> 8;

                    V[0xF] = (V[regX] & 0x80)>>7;
                    V[regX] = V[regX] << 1;
                    break;
                }
            }
            break;
        }
        case 0x0009:{
            uint8_t regX = (opCode & 0x0F00) >> 8;
            uint8_t regY = (opCode & 0x00F0) >> 4;

            if(V[regX] != V[regY]){
                pc+=2;
            }
            break;
        }
        case 0x000A:{
            uint16_t address = opCode & 0x0FFF;

            I = address;
            break;
        }
        case 0x000B:{
            uint16_t address = opCode & 0x0FFF;

            pc = V[0x0] + address;
            break;
        }
        case 0x000C:{ //uses #include <random> from the header file
            uint8_t regX = (opCode & 0x0F00)>>8;
            uint8_t nn = opCode & 0x00FF;
            uint8_t rdNum = dist(gen); // generates the random number

            V[regX] = rdNum & nn;
            break;
        }
        case 0x000D:{ //Draw opcode
            //printf("DRAW OPCODE Executed: %04x\n", opCode);
            uint8_t x = (opCode & 0x0F00) >> 8;
            uint8_t y = (opCode & 0x00F0) >> 4;
            uint8_t heightN = opCode & 0x000F; // extract N which represents sprite height
            
            uint8_t xPos = V[x];
            uint8_t yPos = V[y];

            V[0xF] =0; //resets collision flag
            for(int row = 0; row < heightN; row++){ //go through the rows until height of the sprite
                uint8_t sprByte = memory[I + row]; //sprite byte starts at memory I + the row its on.

                for(int col = 0; col < 8; col++){ //column is < 8 because each sprite byte has 8 bits
                    int pixelX = ((xPos + col) % 64); //using mod to make the sprites wrap around incase of overflow.
                    int pixelY = ((yPos + row) % 32);

                    int pixelIndex = pixelY * 64 + pixelX; // however many Y's/rows then add the X value

                    if(sprByte & (0x80) >> col){ //if the byte has a value there, check. //to further explain, this if checks each bit in the Byte for a value until its finds 1.
                        
                        if(graphics[pixelIndex] == 1){
                            V[0xF] = 1; // sets Vf to 1 
                        }

                        graphics[pixelIndex] ^= 1; //XOR 
                    }
                }
            }
            break;
        }
        case 0x000E:{
            switch(opCode & 0x00FF){
                case 0x009E:{
                    uint8_t regX = (opCode & 0x0F00)>>8;
                    
                    
                    if(key[V[regX]] == 1){
                        pc += 2;
                    }
                    break;
                }
                case 0x00A1:{
                    uint8_t regX = (opCode & 0x0F00)>>8;
                    


                    if(key[V[regX]] != 1){
                        pc += 2;
                    }
                    break;
                }
            }
            break;
        }
        case 0x000F:{
                switch(opCode & 0x00FF){
                    case 0x0007:{
                        uint8_t regX = (opCode & 0x0F00) >> 8;

                        V[regX] = dT;
                        break;
                    }
                    case 0x000A:{
                         uint8_t regX = (opCode & 0x0F00) >> 8;
                        bool keyPress = false;

                        for (int i = 0; i < 16; i++) { //checks keypad if anything is pressed
                            if (key[i] == 1) {
                                V[regX] = i; 
                                keyPress = true;
                                break;
                             }
                        }

                        if (!keyPress) {
                            pc -= 2;
                        }

                        break;
                    }
                    case 0x0015:{
                        uint8_t regX = (opCode & 0x0F00) >> 8;

                        dT = V[regX];
                        break;
                    }
                    case 0x0018:{
                        uint8_t regX = (opCode & 0x0F00) >> 8;

                        sT = V[regX];
                        break;
                    }
                    case 0x001E:{
                        uint8_t regX = (opCode & 0x0F00) >> 8;

                        I = I + V[regX];
                        break;
                    }
                    case 0x0029:{
                         uint8_t regX = (opCode & 0x0F00) >> 8;

                         I = 0x50 + (V[regX] * 5); //font is 5 bytes tall so multiply digit by 5 to find sprite. starting at 0x50
                         break;
                    }
                    case 0x0033:{
                        uint8_t regX = (opCode & 0x0F00) >> 8;

                        memory[I] = V[regX]/100; //ex: 452/100 = 4
                        memory[I + 1] = (V[regX]/10) % 10; // 452/10 = 45, then 45%10 = 5
                        memory[I + 2] = V[regX]%10; //452 % 10 = 2
                        break;
                    }
                    case 0x0055:{
                        uint8_t regX = (opCode & 0x0F00) >> 8;

                        for(int i=0; i<=regX;i++){ //from V0 to Vx which is whatever x provided
                            memory[I + i] = V[i];
                        }
                        break;
                    }
                    case 0x0065:{
                        uint8_t regX = (opCode & 0x0F00) >> 8;
                        
                        for(int i=0; i<=regX;i++){ // <= because needs to get all registers its asking for
                            V[i] = memory[I + i];
                        }
                        break;
                    }
                }
                break;
            }
     break;       
    }
}

void loadGame(const char *filename){ //load file from filename here
    FILE *f1 = fopen(filename, "rb"); //read the file in binary mode

    if(f1 == NULL){
        printf("Error opening file: %s\n", filename);
        return; 
    }

    //find file size
    fseek(f1, 0, SEEK_END);
    long fsize = ftell(f1);
    
    if(fsize > 4096 - 0x200){
        printf("Invalid file size!\n");
        return;
    }
    rewind(f1); //rewind to start of file

    //now read the file starting at 0x200
    fread(&memory[0x200] , 1, fsize, f1);
    fclose(f1); //close the file
}