/*
    @file 2048_game_arduino.ino
    @brief
      Made for playing "2048" on Arduino.
      "2048" is a puzzle game released by Gabriele Siluri in March 2014.
      You can play using serial communication terminal software.
    @author Kei Takagi
    @date 2020.04.12

    Copyright (c) 2020 Kei Takagi
*/

uint8_t Board[4][4];
uint8_t incomingByte;

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(115200);
  initialize();
}


void initialize() {
  int8_t x, y;
  for (y = 0; y < 4; y++)
    for (x = 0; x < 4; x++)
      Board[x][y] = 0;
  Serial.println("START");
  findNewTile();
  drawBoard();
}


void loop() {
  uint8_t compBoard[4][4];
  int8_t x, y;
  for (x = 0; x < 4; x++)
    for (y = 0; y < 4; y++)
      compBoard[x][y] = Board[x][y];

  // Key input
  if (Serial.available() > 0) {
    switch (Serial.read()) {
      case 65:
        up(); break;
      case 66:
        down(); break;
      case 68:
        left(); break;
      case 67:
        right(); break;
      default:
        return;
    }

    // Check for changes
    boolean chkFlg = false;
    for (x = 0; x < 4; x++)
      for (y = 0; y < 4; y++)
        if (compBoard[x][y] != Board[x][y])chkFlg = true;

    if (chkFlg == true)findNewTile();
    drawBoard();
    gameClearCheck();
    gameOverCheck();
  }
}


void up() {
  int8_t x, y , wk;
  for (x = 0; x < 4; x++) {
    wk = 0;

    // Align
    for (y = 0; y < 4; y++)
      if (Board[x][y] != 0)
        Board[x][wk++] = Board[x][y];

    // Fill with zero
    for (y = wk; y < 4; y++)Board[x][y] = 0;

    // Adjacency check
    for (y = 0; y < 3; y++) {
      // check if adjacent tiles have the same value
      if (Board[x][y] > 0 && Board[x][y] == Board[x][y + 1]) {
        // Addition
        Board[x][y]++;
        for (wk = y + 1; wk < 3; wk++)
          Board[x][wk] = Board[x][wk + 1];
        Board[x][3] = 0;
      }
    }
  }
}


void down() {
  int8_t x, y , wk;
  boolean rtnVal = false;
  for (x = 0; x < 4; x++) {
    wk = 3;

    // Align
    for (y = 3; y >= 0; y--)
      if (Board[x][y] != 0)
        Board[x][wk--] = Board[x][y];

    // Fill with zero
    for (y = wk; y >= 0; y--)Board[x][y] = 0;

    // Adjacency check
    for (y = 3; y >= 0; y--) {
      // Check if adjacent tiles have the same value
      if (Board[x][y] > 0 && Board[x][y] == Board[x][y - 1]) {
        // Addition
        Board[x][y]++;
        for (wk = y - 1; wk > 0; wk--)
          Board[x][wk] = Board[x][wk - 1];
        Board[x][0] = 0;
      }
    }
  }
}


void right() {
  int8_t x, y , wk;
  for (y = 0; y < 4; y++) {
    wk = 3;

    // Align
    for (x = 3; x >= 0; x--)
      if (Board[x][y] != 0)
        Board[wk--][y] = Board[x][y];

    // Fill with zero
    for (x = wk; x >= 0; x--)Board[x][y] = 0;

    // Adjacency check
    for (x = 3; x >= 0; x--) {
      // Check if adjacent tiles have the same value
      if (Board[x][y] > 0 && Board[x][y] == Board[x - 1][y]) {
        // Addition
        Board[x][y]++;
        for (wk = x - 1; wk > 0; wk--)
          Board[wk][y] = Board[wk - 1][y];
        Board[0][y] = 0;
      }
    }
  }
}


void left() {
  int8_t x, y , wk;
  for (y = 0; y < 4; y++) {
    wk = 0;

    // Align
    for (x = 0; x < 4; x++)
      if (Board[x][y] != 0)
        Board[wk++][y] = Board[x][y];

    // Fill with zero
    for (x = wk; x < 4; x++)Board[x][y] = 0;

    // Adjacency check
    for (x = 0; x < 3; x++) {
      // check if adjacent tiles have the same value
      if (Board[x][y] > 0 && Board[x][y] == Board[x + 1][y]) {
        // Addition
        Board[x][y]++;
        for (wk = x + 1; wk < 3; wk++)
          Board[wk][y] = Board[wk + 1][y];
        Board[3][y] = 0;
      }
    }
  }
}


void findNewTile() {
  int8_t x, y, wk;
  uint8_t tiles[16];

  // Clear tiles
  for (wk = 0; wk < 16; wk++)
    tiles[wk] = 0;

  // Find an empty tile
  wk = -1;
  for (y = 0; y < 4; y++)
    for (x = 0; x < 4; x++)
      if ( Board[x][y] == 0)
        tiles[++wk] = x + (y * 4);
  if (wk > 0)
    wk = tiles[random(0, wk + 1)];
  else
    wk = tiles[0];
  x = wk % 4;
  y = wk / 4;

  // New tiles appear
  Board[x][y] = random(1, 3);
}


void gameClearCheck() {
  uint8_t x, y, flg = false;
  for (y = 0; y < 4; y++)
    for (x = 0; x < 4; x++)
      if (Board[x][y] == 11)flg = true;
  if (flg) {
    Serial.println("CLEAR");
    initialize();
  }
}


void gameOverCheck() {
  uint8_t x, y, flg = false;

  // Check if adjacent tiles have the same value
  // Column
  for (y = 0; y < 4; y++)
    for (x = 0; x < 3; x++)
      if (Board[x][y] ==  Board[x + 1][y] )flg = true;
  // Row
  for (y = 0; y < 3; y++)
    for (x = 0; x < 4; x++)
      if ( Board[x][y] ==  Board[x][y + 1])flg = true;

  // Find an empty tile
  for (y = 0; y < 4; y++)
    for (x = 0; x < 4; x++)
      if ( Board[x][y] == 0)flg = true;
  if (flg == false) {
    Serial.println("GAMEOVER");
    initialize();
  }
}


void drawBoard() {
  uint8_t x, y;
  for (y = 0; y < 4; y++) {
    for (x = 0; x < 4; x++) {
      Serial.print(Board[x][y], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}
