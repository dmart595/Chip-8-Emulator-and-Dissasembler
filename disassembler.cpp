#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>



//check notes for clarification

void dissasembleChip8(const uint8_t *bufferAddress, int pc){
    const uint8_t *codeRead = &bufferAddress[pc];
    uint8_t firstnib = (codeRead[0] >> 4); //gets the first 4 bits

    printf("%04x %02x %02x  ", pc , codeRead[0], codeRead[1]);
    switch(firstnib)
    {
        case 0x00:{
            uint8_t last2Nibs = codeRead[1];
            switch(last2Nibs){
                default:{
                    uint8_t address = codeRead[0] & 0x0F;
                    printf("%-10s #$%01x%02x", "SYS", address, codeRead[1]); //sys means system call
                    break;
                }
                case 0xE0:{
                    printf("%-10s", "CLS"); //CLS means clear the screen
                    break;
                }
                case 0xEE:{
                    printf("%-10s", "RET"); //RET means return
                    break;
                }
            }
            break;
        }
        case 0x01:{
            uint8_t topByte = codeRead[0] & 0x0F;
            printf("%-10s #$%01x%02x" , "JMP"  ,topByte, codeRead[1]);//JMP means jump
            break;
        }
        case 0x02:{
            uint8_t topByte = codeRead[0] & 0x0F;
            printf("%-10s #$%01x%02x" , "CALL", topByte, codeRead[1]); //CALL means CALL
            break;
        }
        case 0x03:{
            uint8_t topByte = codeRead[0] & 0x0F;
            printf("%-10s V%01x,#$%02x" , "SE" , topByte, codeRead[1]);
            //SE means skip next if equal
            break;
        }
        case 0x04:{
            uint8_t topByte = codeRead[0] & 0x0F;
            printf("%-10s V%01x,#$%02x" , "SNE" , topByte, codeRead[1]); 
            //SNE means skip next if not equal
            break;
        }
        case 0x05:{
            uint8_t topByte = codeRead[0] & 0x0F;
            uint8_t lowerByte = (codeRead[1] & 0xF0) >>4; //shifted 4 bits to get the exact register num
            printf("%-10s V%01x V%01x" , "SE" , topByte, lowerByte);
            break;
        }
        case 0x06: {
            uint8_t reg = codeRead[0] & 0x0F; //gets right 4 bits
            printf("%-10s V%01x ,#$%02x", "MVI", reg, codeRead[1]);
            break;
        }
        case 0x07:{
            uint8_t reg = codeRead[0] & 0x0F;
            printf("%-10s V%01x,#$%02x", "ADD", reg, codeRead[1]); //shows the addition
            break;
        } 
        case 0x08:{
            uint8_t lastNib = codeRead[1] & 0x0F;
            switch(lastNib){
                case 0x0:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    uint8_t regY = (codeRead[1] & 0xF0) >> 4;
                    printf("%-10s V%01x,V%01x", "LD" , regX, regY); //LD means load
                    break;
                    //loads the first V register with the value of the second V.
                }
                case 0x1:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    uint8_t regY = (codeRead[1] & 0xF0) >> 4;
                    printf("%-10s V%01x,V%01x", "OR" , regX, regY); //OR means bitwise OR
                    break;
                }
                case 0x2:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    uint8_t regY = (codeRead[1] & 0xF0) >> 4;
                    printf("%-10s V%01x,V%01x", "AND" , regX, regY); //AND means bitwise AND
                    break;
                }
                case 0x3:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    uint8_t regY = (codeRead[1] & 0xF0) >> 4;
                    printf("%-10s V%01x,V%01x", "XOR" , regX, regY); //OR means bitwise OR
                    break;
                }
                case 0x4:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    uint8_t regY = (codeRead[1] & 0xF0) >> 4;
                    printf("%-10s V%01x,V%01x", "ADD" , regX, regY); //ADD registers and put them in Vx
                    break;
                }
                case 0x5:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    uint8_t regY = (codeRead[1] & 0xF0) >> 4;
                    printf("%-10s V%01x,V%01x", "SUB" , regX, regY); //SUB registers and put them in Vx
                    //Vx - Vy
                    break;
                }
                case 0x6:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    printf("%-10s V%01x", "SHR" , regX); //SHR means shift right 1
                    break;
                }
                case 0x7:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    uint8_t regY = (codeRead[1] & 0xF0) >> 4;
                    printf("%-10s V%01x,V%01x", "SUBN" , regX, regY); //SUB registers and put them in Vx
                    //Vy - Vx
                    break;
                }
                case 0xE:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    printf("%-10s V%01x", "SHL" , regX); //SHL is shift left 1
                    break;
                }
            }
            break;
        }
        case 0x09: {
            uint8_t regX = codeRead[0] & 0x0F;
            uint8_t regY = (codeRead[1] &0xF0)>>4;
            printf("%-10s V%01x,V%01x", "SNE", regX, regY);
            break;
        }
        case 0x0A: {
            uint8_t addressI = codeRead[0] & 0x0F;
            printf("%-10s I ,#$%01x%02x ", "MVI", addressI, codeRead[1]); //mvi is move immediate
            break;
        }
        case 0x0B: {
            uint8_t topByte = codeRead[0] & 0x0F;
            printf("%-10s %01x%02x ", "JMP",topByte, codeRead[1]);
            break; 
        }
        case 0x0C:{
            uint8_t topByte = codeRead[0] & 0x0F;
            printf("%-10s V%01x,#$%02x ", "RND",topByte, codeRead[1]); //rnd means random
            break; 
        }
        case 0x0D:{
            uint8_t topByte = codeRead[0] & 0x0F;
            uint8_t lowerByte = (codeRead[1] & 0xF0) >> 4;
            uint8_t height = (codeRead[1] & 0x0F);
            printf("%-10s V%01x,V%01x,#$%01x", "DRW",topByte, lowerByte, height); //DRW means draw
            break; 
        }
        case 0x0E:{
            uint8_t last2Nibs = codeRead[1];
            switch(last2Nibs){
                default:{
                    uint8_t address = codeRead[0] * 0x0F;
                    printf("%-10s #$%01x%02x", "UNKNOWN", address, codeRead[1]);
                    break;
                }
                case 0x9E:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    printf("%-10s V%01x", "SKP", regX); //SKP means skip if key press
                    break;
                }
                case 0xA1:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    printf("%-10s V%01x", "SKNP", regX); //SKNP means skip if no key press 
                    break;
                }
            }
            break;
        }
        case 0x0F:{
            uint8_t last2Nibs = codeRead[1];
            switch(last2Nibs){
                default:{
                    uint8_t address = codeRead[0] * 0x0F;
                    printf("%-10s #$%01x%02x", "UNKNOWN", address, codeRead[1]);
                    break;
                }
                case 0x07:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    printf("%-10s V%01x,DT", "LD", regX);
                    break;
                    //load a delay timer (DT) value into Vx
                }
                case 0x15:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    printf("%-10s DT,V%01x", "LD", regX); //puts Vx into DT
                    break;
                }
                case 0x18:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    printf("%-10s ST,V%01x", "LD", regX); //puts Vx into ST (sound timer)
                    break;
                }
                case 0x1E:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    printf("%-10s I,V%01x", "ADD", regX); //Adds Vx to I
                    break;
                }
                case 0x29:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    printf("%-10s I,V%01x", "LD", regX); //sets I to location (of font) of digit Vx
                    break;
                }
                case 0x33:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    printf("%-10s V%01x", "BCD", regX); //stores BCD of Vx into i,i+1,i+2
                    break;
                }
                case 0x55:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    printf("%-10s [I],V%01x", "LD", regX); //stores register V0 to Vx at I then I+1 and so on
                    break;
                }
                case 0x65:{
                    uint8_t regX = codeRead[0] & 0x0F;
                    printf("%-10s V%01x,[I]", "LD", regX); //reads V0 to Vx from I then I+1 and so on
                    break;
                }
            }
            break;
        }

    }
}

int main(int argc, char **argv){
    FILE *f1 = fopen(argv[1], "rb");  //lets open the file and read the binary (rb)
    if(f1 == NULL){
        printf("Error on file open \n"); //just in case its wrong
        exit(1);
    }

    //get file size
    fseek(f1, 0 , SEEK_END);
    int f1Size = ftell(f1);
    fseek(f1, 0 , SEEK_SET);

    /*
    Short story: chip-8 programs start storing memory at 0x200.
    Long story:
    For this reason, most programs written for the original system begin at memory location 512 (0x200)
     and do not access any of the memory below the location 512 (0x200).
      The uppermost 256 bytes (0xF00-0xFFF) are reserved for display refresh, and the 96 bytes below that
      (0xEA0-0xEFF) were reserved for the call stack, internal use, and other variables.*/

    //so now lets read the file

    unsigned char *buffer = static_cast<unsigned char*>(malloc(f1Size + 0x200));
    //allocate memory for the file + 0x200 (512 bytes)
    fread(buffer + 0x200, f1Size, 1, f1);
    fclose(f1);

    int pc = 0x200;
    while(pc <f1Size +0x200){
        dissasembleChip8(buffer, pc);
        pc += 2;
        printf("\n");
    }
    return 0;

}
