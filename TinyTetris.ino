
/*  Tiny Tetris by Anthony Russell 17-09-2016*/

/*  V0.91                                     */


/*

To do:

High score functionality.

Better key pad control code.

Add blocks to program memory. 

Decent random number generator.

Create a letter font, create a proper system for rendering numbers and letters.

Tidy up code and optimize for memory, sort out the globals and types.

Create defines for all the magic numbers but they are useful for now.

*/



#include <Wire.h>



#define OLED_ADDRESS	        	0x3C //you may need to change this, this is the OLED I2C address.  
#define OLED_COMMAND	            0x80
#define OLED_DATA	                0x40
#define OLED_DISPLAY_OFF	        0xAE
#define OLED_DISPLAY_ON	            0xAF
#define OLED_NORMAL_DISPLAY	    	0xA6
#define OLED_INVERSE_DISPLAY     	0xA7
#define OLED_SET_BRIGHTNESS	        0x81
#define OLED_SET_ADDRESSING	        0x20
#define OLED_HORIZONTAL_ADDRESSING	0x00 
#define OLED_VERTICAL_ADDRESSING	0x01
#define OLED_PAGE_ADDRESSING	    0x02 

#define OLED_SET_COLUMN             0x21
#define OLED_SET_PAGE	            0x22


#define KEYPAD_PIN	                A0


//The pieces ,  To do: this should go in program memory-

const bool  BlockI[4][4] = { { 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 0, 0 }, };
const bool  BlockJ[4][4] = { { 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 1, 1, 0, 0 },{ 0, 0, 0, 0 }, };
const bool  BlockL[4][4] = { { 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 1, 0 },{ 0, 0, 0, 0 }, };
const bool  BlockO[4][4] = { { 0, 0, 0, 0 },{ 0, 1, 1, 0 },{ 0, 1, 1, 0 },{ 0, 0, 0, 0 }, };
const bool  BlockS[4][4] = { { 0, 0, 0, 0 },{ 0, 1, 1, 0 },{ 1, 1, 0, 0 },{ 0, 0, 0, 0 }, };
const bool  BlockT[4][4] = { { 0, 0, 0, 0 },{ 1, 1, 1, 0 },{ 0, 1, 0, 0 },{ 0, 0, 0, 0 }, };
const bool  BlockZ[4][4] = { { 0, 0, 0, 0 },{ 1, 1, 0, 0 },{ 0, 1, 1, 0 },{ 0, 0, 0, 0 }, };

// To do: need to enable this at some stage
//const bool  BlockI[4][4] PROGMEM  = { { 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 0, 0 }, };
//const bool  BlockJ[4][4] PROGMEM  = { { 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 1, 1, 0, 0 },{ 0, 0, 0, 0 }, };
//const bool  BlockL[4][4] PROGMEM  = { { 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 1, 0 },{ 0, 0, 0, 0 }, };
//const bool  BlockO[4][4] PROGMEM =  { { 0, 0, 0, 0 },{ 0, 1, 1, 0 },{ 0, 1, 1, 0 },{ 0, 0, 0, 0 }, };
//const bool  BlockS[4][4] PROGMEM =  { { 0, 0, 0, 0 },{ 0, 1, 1, 0 },{ 1, 1, 0, 0 },{ 0, 0, 0, 0 }, };
//const bool  BlockT[4][4] PROGMEM =  { { 0, 0, 0, 0 },{ 1, 1, 1, 0 },{ 0, 1, 0, 0 },{ 0, 0, 0, 0 }, };
//const bool  BlockZ[4][4] PROGMEM =  { { 0, 0, 0, 0 },{ 1, 1, 0, 0 },{ 0, 1, 1, 0 },{ 0, 0, 0, 0 }, };


// the numbers for score, To do: create letter fonts

const byte NumberFont[10][8] PROGMEM = {

	{ 0x00, 0x1c, 0x22, 0x26, 0x2a, 0x32, 0x22, 0x1c },
	{ 0x00, 0x1c, 0x08, 0x08, 0x08, 0x08, 0x0c, 0x08 },
	{ 0x00, 0x3e, 0x02, 0x04, 0x18, 0x20, 0x22, 0x1c },
	{ 0x00, 0x1c, 0x22, 0x20, 0x18, 0x20, 0x22, 0x1c },
	{ 0x00, 0x10, 0x10, 0x3e, 0x12, 0x14, 0x18, 0x10 },
	{ 0x00, 0x1c, 0x22, 0x20, 0x20, 0x1e, 0x02, 0x3e },
	{ 0x00, 0x1c, 0x22, 0x22, 0x1e, 0x02, 0x04, 0x18 },
	{ 0x00, 0x04, 0x04, 0x04, 0x08, 0x10, 0x20, 0x3e },
	{ 0x00, 0x1c, 0x22, 0x22, 0x1c, 0x22, 0x22, 0x1c },
	{ 0x00, 0x0c, 0x10, 0x20, 0x3c, 0x22, 0x22, 0x1c }
};



#define KEY_LEFT 1
#define KEY_RIGHT 2
#define KEY_DOWN 3
#define KEY_ROTATE 4  

byte uiKeyLeft = 2;
byte uiKeyRight = 3;
byte uiKeyDown = 4;
byte uiKeyRotate = 5;


//struct for Key press control

struct keyPress
{
	long left;
	long right;
	long down;
	long rotate;
};


//struct for pieces

struct PieceSpace
{
	byte umBlock[4][4];
	char Row;
	char Coloum;
};

//Globals, is a mess. To do: tidy up and reduce glogal use if possible

byte pageArray[8] = { 0 };
byte scoreDisplayBuffer[8][6] = { { 0 },{ 0 } };
byte nextBlockBuffer[8][2] = { { 0 },{ 0 } };
bool optomizePageArray[8] = { 0 };
byte blockColoum[10] = { 0 };
byte tetrisScreen[14][25] = { { 1 } ,{ 1 } };
PieceSpace currentPiece = { 0 };
PieceSpace oldPiece = { 0 };
byte nextPiece = 0;
keyPress key = { 0 };
bool gameOver = false;
unsigned long moveTime = 0;
int pageStart = 0;
int pageEnd = 0;

int score = 0;
int acceleration = 0;
int level = 0;
int levellineCount = 0;
int dropDelay = 1000;


// I2C 

void OLEDCommand(byte command) {

	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_COMMAND);
	Wire.write(command);
	Wire.endTransmission();
}


void OLEDData(byte data) {

	Wire.beginTransmission(OLED_ADDRESS);
	Wire.write(OLED_DATA);
	Wire.write(data);
	Wire.endTransmission();

}



void setup()
{
	Serial.begin(9600);
	while (!Serial) { ; }

	Wire.begin();
	Wire.setClock(400000);

	OLEDCommand(OLED_DISPLAY_OFF);
	delay(20);
	OLEDCommand(OLED_DISPLAY_ON);
	delay(20);
	OLEDCommand(OLED_NORMAL_DISPLAY);
	delay(20);
	OLEDCommand(0x8D);
	delay(20);
	OLEDCommand(0x14);
	delay(20);
	OLEDCommand(OLED_NORMAL_DISPLAY);

	fillTetrisScreen(0);

	randomSeed(analogRead(7)); /// To do: create a decent random number generator. 

	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	delay(100);
	digitalWrite(13, LOW);
	delay(200);
	digitalWrite(13, HIGH);
	delay(50);
	digitalWrite(13, LOW);
}

void fillTetrisArray(byte value)

{
	for (char r = 0; r < 24; r++)
	{
		for (char c = 0; c < 14; c++)

		{
			tetrisScreen[c][r] = value;
		}
	}

	for (char r = 21; r < 24; r++) for (char c = 0; c < 14; c++) tetrisScreen[c][r] = 0; 
}

void fillTetrisScreen(byte value)

{
	for (int r = 1; r < 21; r++)
	{
		for (int c = 2; c < 12; c++)

		{
			tetrisScreen[c][r] = value;
		}
	}
}

void drawTetrisScreen()
{


	for (byte r = 1; r < 21; r++)

	{
		//loop through rows to see if there is data to be sent

		for (byte c = 2; c < 12; c++)

		{

			if ((tetrisScreen[c][r] == 2) | (tetrisScreen[c][r] == 3))

			{
				//send line to screen
				for (byte i = 0; i < 10; i++)/// i=2 i<12!!

				{
					blockColoum[i] = tetrisScreen[i + 2][r];

					//clear delete block
					if (tetrisScreen[i + 2][r] == 3) tetrisScreen[i + 2][r] = 0;

				}
				drawTetrisLine((r - 1) * 6);
				break; break;
			}

		}


	}




}

void drawTetrisLine(byte x)

{
	//fill array with blocks based on blockRow

	//clear page and Optimize array
	memset(optomizePageArray, 0, 8);   ///review this... declare them here? interesting question...
	memset(pageArray, 0, 8);

	x++; // up one


	//*********Column 0***********

	//draw block
	if (blockColoum[0] == 2 | blockColoum[0] == 1)
	{
		pageArray[0] = pageArray[0] | B11111001;
		optomizePageArray[0] = 1;

	}

	//delete block
	if (blockColoum[0] == 3)
	{
		pageArray[0] = pageArray[0] | B00000001; //create side wall
		pageArray[0] = pageArray[0] & B00000111;
		optomizePageArray[0] = 1;
	}



	//*********Column 1***********
	if (blockColoum[1] == 2 | blockColoum[1] == 1)
	{
		pageArray[1] = pageArray[1] | B00111110;
		optomizePageArray[1] = 1;
	}

	//delete block
	if (blockColoum[1] == 3)
	{
		pageArray[1] = pageArray[1] & B11000001;
		optomizePageArray[1] = 1;
	}



	//*********Column 2***********
	if (blockColoum[2] == 2 | blockColoum[2] == 1)
	{
		pageArray[1] = pageArray[1] | B10000000;
		optomizePageArray[1] = 1;


		pageArray[2] = pageArray[2] | B00001111;
		optomizePageArray[2] = 1;

	}

	//delete block
	if (blockColoum[2] == 3)
	{
		pageArray[1] = pageArray[1] & B01111111;
		optomizePageArray[1] = 1;

		pageArray[2] = pageArray[2] & B11110000;
		optomizePageArray[2] = 1;

	}



	//*********Column 3***********
	if (blockColoum[3] == 2 | blockColoum[3] == 1)
	{
		pageArray[2] = pageArray[2] | B11100000;
		optomizePageArray[2] = 1;

		pageArray[3] = pageArray[3] | B00000011;
		optomizePageArray[3] = 1;

	}

	//delete block
	if (blockColoum[3] == 3)
	{
		pageArray[2] = pageArray[2] & B00011111;
		optomizePageArray[2] = 1;

		pageArray[3] = pageArray[3] & B11111100;
		optomizePageArray[3] = 1;

	}



	//*********Column 4***********
	if (blockColoum[4] == 2 | blockColoum[4] == 1)
	{
		pageArray[3] = pageArray[3] | B11111000;
		optomizePageArray[3] = 1;

	}
	//delete block
	if (blockColoum[4] == 3)
	{
		pageArray[3] = pageArray[3] & B00000111;
		optomizePageArray[3] = 1;

	}



	//*********Column 5***********

	if (blockColoum[5] == 2 | blockColoum[5] == 1)
	{
		pageArray[4] = pageArray[4] | B00111110;
		optomizePageArray[4] = 1;

	}

	//delete block
	if (blockColoum[5] == 3)
	{
		pageArray[4] = pageArray[4] & B11000001;
		optomizePageArray[4] = 1;

	}



	//*********Column 6***********
	if (blockColoum[6] == 2 | blockColoum[6] == 1)
	{
		pageArray[4] = pageArray[4] | B10000000;
		optomizePageArray[4] = 1;

		pageArray[5] = pageArray[5] | B00001111;
		optomizePageArray[5] = 1;

	}
	//delete block
	if (blockColoum[6] == 3)
	{
		pageArray[4] = pageArray[4] & B01111111;
		optomizePageArray[4] = 1;

		pageArray[5] = pageArray[5] & B11110000;
		optomizePageArray[5] = 1;

	}




	//*********Column 7***********
	if (blockColoum[7] == 2 | blockColoum[7] == 1)
	{
		pageArray[5] = pageArray[5] | B11100000;
		optomizePageArray[5] = 1;

		pageArray[6] = pageArray[6] | B00000011;
		optomizePageArray[6] = 1;

	}

	if (blockColoum[7] == 3)
	{
		pageArray[5] = pageArray[5] & B00011111;
		optomizePageArray[5] = 1;

		pageArray[6] = pageArray[6] & B11111100;
		optomizePageArray[6] = 1;

	}



	//*********Column 8***********
	if (blockColoum[8] == 2 | blockColoum[8] == 1)
	{
		pageArray[6] = pageArray[6] | B11111000;
		optomizePageArray[6] = 1;

	}

	//delete block
	if (blockColoum[8] == 3)
	{
		pageArray[6] = pageArray[6] & B00000111;
		optomizePageArray[6] = 1;

	}




	//*********Column 9***********
	if (blockColoum[9] == 2 | blockColoum[9] == 1)
	{
		pageArray[7] = pageArray[7] | B10111110;
		optomizePageArray[7] = 1;
	}

	if (blockColoum[9] == 3)
	{
		pageArray[7] = pageArray[7] | B10000000;//create side wall
		pageArray[7] = pageArray[7] & B11000001;
		optomizePageArray[7] = 1;
	}




	//Optimize - figure out what page array has data 

	for (int page = 0; page < 8; page++)

	{
		if (optomizePageArray[page])
		{
			//block found set page start
			pageStart = page;
			break;
		}

	}
	for (int page = 7; page >= 0; page--)

	{
		if (optomizePageArray[page])
		{
			//block found set page end
			pageEnd = page;
			break;
		}

	}


	//set Vertical addressing mode and column - page start end
	OLEDCommand(OLED_SET_ADDRESSING);
	OLEDCommand(OLED_VERTICAL_ADDRESSING);

	OLEDCommand(OLED_SET_COLUMN);
	OLEDCommand(x);
	OLEDCommand(x + 4);

	OLEDCommand(OLED_SET_PAGE);
	OLEDCommand(pageStart);
	OLEDCommand(pageEnd);

	//send the array 5 times  

	for (int c = 0; c <5; c++)
	{
		for (int p = pageStart; p <= pageEnd; p++)
		{
			OLEDData(pageArray[p]);
		}

	}

}

void loadPiece(byte peiceNumber, byte row, byte coloum, bool loadScreen)


{
	//load the piece from piece array to screen
	byte pieceRow = 0;
	byte pieceColoum = 0;
	byte c = 0;


	switch (peiceNumber)
	{
	case 1: memcpy(currentPiece.umBlock, BlockI, 16); break;
	case 2: memcpy(currentPiece.umBlock, BlockJ, 16); break;
	case 3: memcpy(currentPiece.umBlock, BlockL, 16); break;
	case 4: memcpy(currentPiece.umBlock, BlockO, 16); break;
	case 5: memcpy(currentPiece.umBlock, BlockS, 16); break;
	case 6: memcpy(currentPiece.umBlock, BlockT, 16); break;
	case 7: memcpy(currentPiece.umBlock, BlockZ, 16); break;
	}

	currentPiece.Row = row;
	currentPiece.Coloum = coloum;


	if (loadScreen) {

		oldPiece = currentPiece;

		for (c = coloum; c < coloum + 4; c++)
		{
			for (int r = row; r < row + 4; r++) {
				if (currentPiece.umBlock[pieceColoum][pieceRow]) tetrisScreen[c][r] = 2;
				pieceRow++;
			}
			pieceRow = 0;
			pieceColoum++;
		}
	}

}

void drawPiece()


{

	char coloum;
	char row;
	byte pieceRow = 0;
	byte pieceColoum = 0;
	char c = 0;

	// delete blocks first

	coloum = oldPiece.Coloum;
	row = oldPiece.Row;

	for (c = coloum; c < coloum + 4; c++)
	{
		for (char r = row; r < row + 4; r++) {
			if (oldPiece.umBlock[pieceColoum][pieceRow]) tetrisScreen[c][r] = 3;
			pieceRow++;
		}
		pieceRow = 0;
		pieceColoum++;
	}

	//draw new blocks
	pieceRow = 0;
	pieceColoum = 0;
	c = 0;

	coloum = currentPiece.Coloum;
	row = currentPiece.Row;

	for (c = coloum; c < coloum + 4; c++)
	{
		for (char r = row; r < row + 4; r++) {
			if (currentPiece.umBlock[pieceColoum][pieceRow]) tetrisScreen[c][r] = 2;
			pieceRow++;
		}
		pieceRow = 0;
		pieceColoum++;
	}
}

void drawLandedPiece()


{

	char coloum;
	char row;
	byte pieceRow = 0;
	byte pieceColoum = 0;
	char c = 0;

	// Landed pieces are 1

	coloum = currentPiece.Coloum;
	row = currentPiece.Row;

	for (c = coloum; c < coloum + 4; c++)
	{
		for (int r = row; r < row + 4; r++) {
			if (currentPiece.umBlock[pieceColoum][pieceRow]) tetrisScreen[c][r] = 1;
			pieceRow++;
		}
		pieceRow = 0;
		pieceColoum++;
	}

	processCompletedLines();
}

bool led = true;

void RotatePiece()
{
	byte i, j;

	byte umFig[4][4] = { 0 };

	memcpy(oldPiece.umBlock, currentPiece.umBlock, 16);
	oldPiece.Row = currentPiece.Row;
	oldPiece.Coloum = currentPiece.Coloum;

	for (i = 0; i < 4; ++i)
		for (j = 0; j < 4; ++j)
			umFig[j][i] = currentPiece.umBlock[4 - i - 1][j];

	oldPiece = currentPiece;
	memcpy(currentPiece.umBlock, umFig, 16);

	if (checkColloision()) currentPiece = oldPiece;

	// no need for this...
	if (led) { digitalWrite(13, HIGH); led = false; }
	delay(1);
	digitalWrite(13, LOW);
	if (led == false) { digitalWrite(13, LOW); led = true; }


}

bool movePieceDown()
{
	bool pieceLanded = false;
	char rndPiece = 0;

	oldPiece = currentPiece;

	currentPiece.Row = currentPiece.Row - 1;

	//check collision

	if (checkColloision()) {

		// its at the bottom make it a landed piece and start new piece	
		currentPiece = oldPiece; // back to where it was
		drawLandedPiece();
		pieceLanded = true;

	}
	if (pieceLanded)

	{

		loadPiece(nextPiece, 19, 4, false);
		acceleration = 0;

		if (checkColloision()) gameOver = true;
		else
		{

			loadPiece(nextPiece, 19, 4, true);
			acceleration = 0;//reset acceleration as there is a new piece
		}

		nextPiece = random(1, 7);
		setNextBlock(nextPiece);

	}
}

void movePieceLeft() {

	oldPiece = currentPiece;

	currentPiece.Coloum = currentPiece.Coloum - 1;

	//check collision

	if (checkColloision()) 	currentPiece = oldPiece; // back to where it was	
}

void movePieceRight() {

	oldPiece = currentPiece;

	currentPiece.Coloum = currentPiece.Coloum + 1;

	//check collision

	if (checkColloision()) 	currentPiece = oldPiece; // back to where it was	
}

bool checkColloision()
{

	byte pieceRow = 0;
	byte pieceColoum = 0;
	char c = 0;
	char coloum = currentPiece.Coloum;
	char row = currentPiece.Row;

	//scan across piece and translate to Tetris array and check Collisions.


	for (c = coloum; c < coloum + 4; c++)
	{
		for (char r = row; r < row + 4; r++) {
			if (currentPiece.umBlock[pieceColoum][pieceRow])
			{

				if (tetrisScreen[c][r] == 1) return true; //is it on landed blocks?

			}
			pieceRow++;
		}

		pieceRow = 0;
		pieceColoum++;


	}

	return false;

}

void processCompletedLines() {

	char rowCheck = 0;
	char coloumCheck = 0;
	bool fullLine = false;
	bool noLine = true;
	char linesProcessed = 0;
	char clearedLines = 0;
	char topRow = 0;
	char bottomRow = 0;
	char currentRow = 0;
	int amountScored = 0;

	if (currentPiece.Row < 1)bottomRow = 1;
	else bottomRow = currentPiece.Row;

	for (int rowCheck = bottomRow; rowCheck < currentPiece.Row + 4; rowCheck++)

	{

		bool fullLine = true;

		for (coloumCheck = 2; coloumCheck < 12; coloumCheck++) {
			if (tetrisScreen[coloumCheck][rowCheck] == 0) { fullLine = false; break; }
		}

		if (fullLine)
		{
			//make line values 3's and render 
			for (char c = 2; c < 12; c++) tetrisScreen[c][rowCheck] = 3;

			bottomRow = rowCheck + 1;
			//line is now all 0's
			linesProcessed++;

			delay(77); // animation :)

		}

		drawTetrisScreen();


	}

	//******all lines are 0's and have been removed from the screen

	if (linesProcessed)


	{

		clearedLines = linesProcessed;

		while (clearedLines) {


			for (currentRow = 1; currentRow < 20; currentRow++) {
				noLine = true;
				for (char c = 2; c < 12; c++)
				{
					if (tetrisScreen[c][currentRow])  noLine = false;
				}
				if (noLine) {
					//move all lines down

					for (int r = currentRow + 1; r < 20; r++) {
						for (char c = 2; c < 12; c++) {

							if (tetrisScreen[c][r]) tetrisScreen[c][r - 1] = 2;
							else tetrisScreen[c][r - 1] = 3;
						}

					}

				}
			}


			//make the 2's 1's
			for (char r = 1; r < 24; r++) {
				for (char c = 2; c < 12; c++) {

					if (tetrisScreen[c][r] == 2)tetrisScreen[c][r] = 1;
				}
			}
			clearedLines--;
			drawTetrisScreen();
		}
	}

	// ************** process score ******************* 
	switch (linesProcessed)
	{
	case 1: 	amountScored = 40 * (level + 1); break;
	case 2: 	amountScored = 100 * (level + 1); break;
	case 3: 	amountScored = 300 * (level + 1); break;
	case 4: 	amountScored = 1200 * (level + 1);

		//do 4 line affect

		OLEDCommand(OLED_INVERSE_DISPLAY);
		delay(100);
		OLEDCommand(OLED_NORMAL_DISPLAY);

		break;
	}

	//score animation
	for (long s = score; s < score + amountScored; s = s + (20 * (level+1))) setScore(s, false);

	score = score + amountScored;
	setScore(score, false);

	//****update level line count

	levellineCount = levellineCount + linesProcessed;
	if (levellineCount > 10) {
		level++;
		levellineCount = 0;

		//do level up affect

		OLEDCommand(OLED_INVERSE_DISPLAY);
		delay(100);
		OLEDCommand(OLED_NORMAL_DISPLAY);
		delay(100);
		OLEDCommand(OLED_INVERSE_DISPLAY);
		delay(100);
		OLEDCommand(OLED_NORMAL_DISPLAY);

	}

	//make the 2's 1's 

	for (char r = bottomRow; r <= topRow; r++) {

		for (char c = 2; c < 12; c++) {
			if (tetrisScreen[c][r]) tetrisScreen[c][r] = 1;

		}

	}

}

void tetrisScreenToSerial()

{
	//for debug
	for (int r = 0; r < 24; r++)
	{
		for (int c = 0; c < 14; c++)
		{
			Serial.print(tetrisScreen[c][r], DEC);
		}

		Serial.println();

	}

	Serial.println();

}

long keytimer = 0;
bool processKey = true;
int Debounce = 0;

bool processKeys()

{
	// not happy with this, To do: sort this out and get the movment right!

	char uiKeyCode = 0;
	bool keypressed = true;
	int leftRight = 300 - acceleration;
	int rotate = 700;
	int down = 110 - acceleration;
	int analogKey = analogRead(KEYPAD_PIN);
	


	if ((analogKey > -1) && (analogKey < 5)) {


		Debounce++;

		if (Debounce > 10) {

			if (processKey) {

				uiKeyCode = KEY_LEFT; //key will be processed immediately
				key.left = millis();

			}

			if (millis() < key.left + leftRight) processKey = false;
			else {
				processKey = true;
				acceleration = acceleration + 70;
				if (acceleration > leftRight) acceleration = leftRight;
			}

		}

	}

	else if ((analogKey < 505) && (analogKey > 500)) {


		Debounce++;

		if (Debounce > 10) {

			if (processKey) {

				uiKeyCode = KEY_RIGHT; //key will be processed immediately
				key.right = millis();

			}

			if (millis() < key.right + leftRight) processKey = false;
			else {
				processKey = true;
				acceleration = acceleration + 70;
				if (acceleration > leftRight) acceleration = leftRight;
			}

		}
	}

	else if ((analogKey < 330) && (analogKey > 324)) {


		Debounce++;

		if (Debounce > 10) {

			if (processKey) {

				uiKeyCode = KEY_DOWN; //key will be processed immediately
				key.down = millis();

			}

			if (millis() < key.down + down) processKey = false;
			else {
				processKey = true;
				acceleration = acceleration + 40;
				if (acceleration > down) acceleration = down;

			}

		}
	}

	else if ((analogKey < 745) && (analogKey > 735)) {

		Debounce++;

		if (Debounce > 10) {

			if (processKey) {

				uiKeyCode = KEY_ROTATE; //key will be processed immediately
				key.rotate = millis();

			}

			if (millis() < key.rotate + rotate) processKey = false;
			else processKey = true;
		}

	}

	else {
		acceleration = 0; processKey = true; Debounce = 0;
	}




	switch (uiKeyCode)
	{
	case KEY_LEFT: movePieceLeft();  break;
	case KEY_RIGHT: movePieceRight(); break;
	case KEY_DOWN:  movePieceDown(); break;
	case KEY_ROTATE: RotatePiece(); break;
	default: keypressed = false;  break;
	}


	if (keypressed)
	{
		drawPiece();
		drawTetrisScreen();
	}
}

void setScore(long score, bool blank)

{
	// this is a kludge. To do: create a proper system for rendering numbers and letters.
	
	
	long ones = (score % 10);
	long tens = ((score / 10) % 10);
	long hundreds = ((score / 100) % 10);
	long thousands = ((score / 1000) % 10);
	long tenthousands = ((score / 10000) % 10);
	long hunderedthousands = ((score / 100000) % 10);

	Serial.println(ones);
	Serial.println(tens);
	Serial.println(hundreds);
	Serial.println(thousands);
	Serial.println(tenthousands);
	Serial.println(hunderedthousands);

	//create the score in upper left part of the screen
	byte font = 0;
	char bytes_out[8];
	memset(scoreDisplayBuffer, 0, sizeof scoreDisplayBuffer);

	//****************score digit 6****************

	for (int v = 0; v<8; v++) bytes_out[v] = pgm_read_byte(&NumberFont[hunderedthousands][v]);

	//write the number to the Score buffer
	for (int i = 0; i < 8; i++)
	{
		scoreDisplayBuffer[i][0] = scoreDisplayBuffer[i][0] | bytes_out[i] >> 1;
	}

	//****************score digit 5****************

	for (int v = 0; v<8; v++) bytes_out[v] = pgm_read_byte(&NumberFont[tenthousands][v]);

	//write the number to the Score buffer
	for (int i = 0; i < 8; i++)
	{
		scoreDisplayBuffer[i][0] = scoreDisplayBuffer[i][0] | (bytes_out[i] << 6);
	}

	//write the number to the Score buffer
	for (int i = 0; i < 8; i++)
	{
		scoreDisplayBuffer[i][1] = scoreDisplayBuffer[i][1] | bytes_out[i] >> 1;
	}

	//****************score digit 4****************

	for (int v = 0; v<8; v++) bytes_out[v] = pgm_read_byte(&NumberFont[thousands][v]);


	//write the number to the Score buffer
	for (int i = 0; i < 8; i++)
	{
		scoreDisplayBuffer[i][1] = scoreDisplayBuffer[i][1] | (bytes_out[i] << 6);
	}

	//write the number to the Score buffer
	for (int i = 0; i < 8; i++)
	{
		scoreDisplayBuffer[i][2] = scoreDisplayBuffer[i][2] | bytes_out[i] >> 1;
	}

	//****************score digit 3****************

	for (int v = 0; v<8; v++) bytes_out[v] = pgm_read_byte(&NumberFont[hundreds][v]);

	//write the number to the Score buffer
	for (int i = 0; i < 8; i++)
	{
		scoreDisplayBuffer[i][2] = scoreDisplayBuffer[i][2] | (bytes_out[i] << 6);
	}

	//write the number to the Score buffer
	for (int i = 0; i < 8; i++)
	{
		scoreDisplayBuffer[i][3] = scoreDisplayBuffer[i][3] | bytes_out[i] >> 1;
	}


	//****************score digit 2****************

	for (int v = 0; v<8; v++) bytes_out[v] = pgm_read_byte(&NumberFont[tens][v]);

	//write the number to the Score buffer
	for (int i = 0; i < 8; i++)
	{
		scoreDisplayBuffer[i][3] = scoreDisplayBuffer[i][3] | (bytes_out[i] << 6);
	}

	//write the number to the Score buffer
	for (int i = 0; i < 8; i++)
	{
		scoreDisplayBuffer[i][4] = scoreDisplayBuffer[i][4] | bytes_out[i] >> 1;
	}


	//****************score digit 1****************

	for (int v = 0; v<8; v++) bytes_out[v] = pgm_read_byte(&NumberFont[ones][v]);

	//write the number to the Score buffer
	for (int i = 0; i < 8; i++)
	{
		scoreDisplayBuffer[i][4] = scoreDisplayBuffer[i][4] | (bytes_out[i] << 6);
	}

	//write the number to the Score buffer
	for (int i = 0; i < 8; i++)
	{
		scoreDisplayBuffer[i][5] = scoreDisplayBuffer[i][5] | bytes_out[i] >> 1;

	}

	//set Vertical addressing mode and column - page start end
	OLEDCommand(OLED_SET_ADDRESSING);
	OLEDCommand(OLED_VERTICAL_ADDRESSING);

	OLEDCommand(OLED_SET_COLUMN);
	OLEDCommand(120);                 //Set column start
	OLEDCommand(127);                 //Set column end

	OLEDCommand(OLED_SET_PAGE);
	OLEDCommand(0);                  //Set page start
	OLEDCommand(5);                  //Set page end

	for (int p = 0; p < 8; p++)
	{
		for (int c = 0; c <6; c++)
		{
			if (blank) OLEDData(0);
			else OLEDData(scoreDisplayBuffer[p][c]);
		}

	}
}

void setNextBlock(byte peiceNumber)

{
	memset(nextBlockBuffer, 0, sizeof nextBlockBuffer); //clear buffer
	switch (peiceNumber)
	{
	case 1:

		//************l piece - 1 *************
		for (int k = 2; k < 6; k++)
		{
			nextBlockBuffer[k][0] = B01110111;
			nextBlockBuffer[k][1] = B01110111;
		}

		break;

	case 2:

		//************J piece - 2 *************
		for (int k = 0; k < 3; k++)
		{
			nextBlockBuffer[k][0] = B01110000;
			nextBlockBuffer[k][1] = B01110111;
		}

		for (int k = 4; k < 7; k++)
		{
			nextBlockBuffer[k][0] = B01110000;
		}
		break;
	case 3:
		//************L piece - 3 *************
		for (int k = 0; k < 3; k++)
		{
			nextBlockBuffer[k][0] = B01110000;
		}

		for (int k = 4; k < 7; k++)
		{
			nextBlockBuffer[k][0] = B01110000;
			nextBlockBuffer[k][1] = B01110111;
		}

		break;

	case 4:

		//************O piece - 4 *************
		for (int k = 0; k < 3; k++)
		{
			nextBlockBuffer[k][0] = B01110000;
			nextBlockBuffer[k][1] = B00000111;
		}

		for (int k = 4; k < 7; k++)
		{
			nextBlockBuffer[k][0] = B01110000;
			nextBlockBuffer[k][1] = B00000111;
		}

		break;

	case 5:

		//************S piece - 5 *************
		for (int k = 0; k < 3; k++)
		{
			nextBlockBuffer[k][0] = B01110000;
			nextBlockBuffer[k][1] = B00000111;
		}

		for (int k = 4; k < 7; k++)
		{
			nextBlockBuffer[k][0] = B00000000;
			nextBlockBuffer[k][1] = B11101110;
		}

		break;


	case 6:

		//************T piece - 6 *************
		for (int k = 0; k < 3; k++)
		{
			nextBlockBuffer[k][0] = B01110000;
			nextBlockBuffer[k][1] = B01110111;
		}

		for (int k = 4; k < 7; k++)
		{
			nextBlockBuffer[k][0] = B00000000;
			nextBlockBuffer[k][1] = B00001110;
		}

		break;

	case 7:

		//************Z piece - 7 *************
		for (int k = 0; k < 3; k++)
		{
			nextBlockBuffer[k][0] = B01110000;
			nextBlockBuffer[k][1] = B00000111;
		}

		for (int k = 4; k < 7; k++)
		{
			nextBlockBuffer[k][0] = B11101110;
			nextBlockBuffer[k][1] = B00000000;
		}

		break;
	}


	//set Vertical addressing mode and column - page start end
	OLEDCommand(OLED_SET_ADDRESSING);
	OLEDCommand(OLED_VERTICAL_ADDRESSING);

	OLEDCommand(OLED_SET_COLUMN);
	OLEDCommand(120);                 //Set column start
	OLEDCommand(127);                 //Set column end

	OLEDCommand(OLED_SET_PAGE);
	OLEDCommand(6);                  //Set page start
	OLEDCommand(7);                  //Set page end


	for (int p = 0; p < 8; p++)
	{
		for (int c = 0; c <2; c++)
		{
			OLEDData(nextBlockBuffer[p][c]);
		}

	}

}

void drawBottom()

{

	//set Vertical addressing mode and column - page start end
	OLEDCommand(OLED_SET_ADDRESSING);
	OLEDCommand(OLED_VERTICAL_ADDRESSING);

	OLEDCommand(OLED_SET_COLUMN);
	OLEDCommand(0);              //Set column start
	OLEDCommand(0);              //Set column end

	OLEDCommand(OLED_SET_PAGE);
	OLEDCommand(0);              //Set page start
	OLEDCommand(7);              //Set page end

	for (int c = 0; c <8; c++)
	{

		OLEDData(255);
	}
}

void drawSides()

{

	//set Vertical addressing mode and column - page start end
	OLEDCommand(OLED_SET_ADDRESSING);
	OLEDCommand(OLED_VERTICAL_ADDRESSING);


	OLEDCommand(OLED_SET_COLUMN);
	OLEDCommand(0);                //Set column start
	OLEDCommand(127);              //Set column end

	OLEDCommand(OLED_SET_PAGE);
	OLEDCommand(0);               //Set page start
	OLEDCommand(7);               //Set page end

	for (int r = 0; r < 128; r++) {

		for (int c = 0; c < 8; c++)
		{
			if (c == 0) OLEDData(1);
			else if (c == 7) OLEDData(128);
			else OLEDData(0);
		}

	}
}


void loop()
{
	//main loop code
	//To do: create high score system that savees to EEprom
	
	
	gameOver = false;
	score = 0;
	fillTetrisArray(1); //fill with 1's to make border
	fillTetrisScreen(2);
	drawTetrisScreen();
	delay(200);
	fillTetrisScreen(3);
	drawTetrisScreen();
	delay(200);
	drawSides();
	drawBottom();

	tetrisScreenToSerial();

	OLEDCommand(OLED_INVERSE_DISPLAY);
	delay(200);
	OLEDCommand(OLED_NORMAL_DISPLAY);

	loadPiece(random(1, 7), 20, 5, true);
	drawTetrisScreen();
	nextPiece = random(1, 7);
	setNextBlock(nextPiece);

	setScore(0, false);
	delay(300);
	setScore(0, true);
	delay(300);
	setScore(0, false);
	byte rnd = 0;

	while (!gameOver)

	{
		movePieceDown();
		drawPiece();
		drawTetrisScreen();
		moveTime = millis();
		while (millis() - moveTime<(dropDelay - (level * 50))) processKeys();
	}

}