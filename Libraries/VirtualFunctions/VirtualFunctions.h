#include "Map.h"
#include "Arduino.h"

const uint8_t kNumberRows = 11, kNumberColumns = 11;
uint8_t current_row = 1, current_column = 1;
char kVirtualMap[11][11] = { {'.','W','.','W','.','W','.','W','.','W','.'},
                            {'W','R','W','B','W',' ','W',' ','.','2','W'},
                            {'.','.','.','.','.','.','.','.','.','.','.'},
                            {'W',' ','.',' ','.',' ','.',' ','W',' ','W'},
                            {'.','W','.','.','.','W','.','.','.','.','.'},
                            {'W','B','W','3','W',' ','W',' ','.',' ','W'},
                            {'.','.','.','.','.','W','.','W','.','.','.'},
                            {'W',' ','.',' ','.','1','.','1','.',' ','W'},
                            {'.','W','.','.','.','.','.','.','.','.','.'},
                            {'W',' ','W',' ','W',' ','.',' ','W',' ','W'},
                            {'.','W','.','W','.','W','.','W','.','W','.'} };

void printVirtualMap() {
  for(uint8_t row = 0 ; row < kNumberRows ; ++row) {
    for(uint8_t column = 0 ; column < kNumberColumns ; ++column) {
      Serial.print(kVirtualMap[row][column]);
      if(column < kNumberColumns - 1) {
        Serial.print(" ");
      }
    }
    Serial.println("");
  }
}

void moveMapNorth() {
  kVirtualMap[current_row][current_column] = 'V';
  if (kVirtualMap[current_row - 2][current_column] != '1' && kVirtualMap[current_row - 2][current_column] != '2' && kVirtualMap[current_row - 2][current_column] != '3') {
    kVirtualMap[current_row - 2][current_column] = 'R';
  }
  current_row -= 2;
}

void moveMapEast() {
  kVirtualMap[current_row][current_column] = 'V';
  if (kVirtualMap[current_row][current_column + 2] != '1' && kVirtualMap[current_row][current_column + 2] != '2' && kVirtualMap[current_row][current_column + 2] != '3') {
    kVirtualMap[current_row][current_column + 2] = 'R';
  }
  current_column += 2;
}

void moveMapSouth() {
  kVirtualMap[current_row][current_column] = 'V';
  if (kVirtualMap[current_row + 2][current_column] != '1' && kVirtualMap[current_row + 2][current_column] != '2' && kVirtualMap[current_row + 2][current_column] != '3') {
    kVirtualMap[current_row + 2][current_column] = 'R';
  }
  current_row += 2;
}

void moveMapWest() {
  kVirtualMap[current_row][current_column] = 'V';
  if (kVirtualMap[current_row][current_column - 2] != '1' && kVirtualMap[current_row][current_column - 2] != '2' && kVirtualMap[current_row][current_column - 2] != '3') {
    kVirtualMap[current_row][current_column - 2] = 'R';
  }
  current_column -= 2;
}

void printMap(Map tiles_map, const uint8_t iFloor) {
  char char_map[tiles_map.getRows()*2 + 1][tiles_map.getColumns()*2 + 1];
  int row_char, column_char;

  for(int i = 0; i < tiles_map.getRows()*2 + 1; ++i) {
    for(int j = 0; j < tiles_map.getColumns()*2 + 1; ++j) {
      char_map[i][j] = '.';
    }
  }
  
  row_char = 1;

  for (int row_tile = 0 ; row_tile < tiles_map.getRows() ; row_tile++) {
    column_char = 1;
    for (int column_tile = 0 ; column_tile < tiles_map.getColumns() ; column_tile++) {
      if(tiles_map.getTile(row_tile, column_tile).getStart()) {
        char_map[row_char][column_char] = 'S';
      }
      else if(tiles_map.getTile(row_tile, column_tile).isVisited()) {
        char_map[row_char][column_char] = 'V';
      }
      else if(tiles_map.getTile(row_tile, column_tile).isAccessible()) {
        char_map[row_char][column_char] = ' ';
      }
      else {
        char_map[row_char][column_char] = 'X';
      }
      if(tiles_map.getTile(row_tile, column_tile).getWeight() == 5) {
        char_map[row_char][column_char] = '1';
      }
      else if(tiles_map.getTile(row_tile, column_tile).getWeight() == 9) {
        char_map[row_char][column_char] = '2';
      }
      else if(tiles_map.getTile(row_tile, column_tile).getWeight() == 13) {
        char_map[row_char][column_char] = '3';
      }
      else if(tiles_map.getTile(row_tile, column_tile).isBlack()) {
        char_map[row_char][column_char] = 'B';
      }
      if(tiles_map.getTile(row_tile, column_tile).getNorth()) {
        char_map[row_char - 1][column_char] = '.';
      }
      else {
        char_map[row_char - 1][column_char] = 'W';
      }
      if(tiles_map.getTile(row_tile, column_tile).getEast()) {
        char_map[row_char][column_char + 1] = '.';
      }
      else {
        char_map[row_char][column_char + 1] = 'W';
      }
      if(tiles_map.getTile(row_tile, column_tile).getSouth()) {
        char_map[row_char + 1][column_char] = '.';
      }
      else {
        char_map[row_char+1][column_char] = 'W';
      }
      if(tiles_map.getTile(row_tile, column_tile).getWest()) {
        char_map[row_char][column_char - 1] = '.';
      }
      else {
        char_map[row_char][column_char - 1] = 'W';
      }
      column_char += 2;
    }
    row_char += 2;
  }

  char_map[tiles_map.getCurrRow()*2 + 1][tiles_map.getCurrColumn()*2 + 1] = 'R';
  
  for(int i = 0 ; i < tiles_map.getRows()*2 + 1 ; i++) {
    for(int j = 0 ; j < tiles_map.getColumns()*2 + 1 ; j++) {
      Serial.print(char_map[i][j]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
  Serial.println(" ");
}

Map followPath(TVector<char> path, Map tiles_map) {
  while (path.getSize() > 0) {
    if (path[0] == 'N') {
      tiles_map.moveNorth();
      moveMapNorth();
    }
    else if (path[0] == 'E') {
      tiles_map.moveEast();
      moveMapEast();
    }
    else if (path[0] == 'S') {
      tiles_map.moveSouth();
      moveMapSouth();
    }
    else if (path[0] == 'W') {
      tiles_map.moveWest();
      moveMapWest();
    }

    Serial.println("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||");
    printVirtualMap();
    Serial.println("");

    path.popFront();
  }

  return tiles_map;
}

Map updateTiles(Map tiles_map) {
  Tile current_tile;

  current_tile = tiles_map.currTile();
    current_tile.setVisited();

    if (kVirtualMap[tiles_map.getCurrRow()*2 + 1][tiles_map.getCurrColumn()*2 + 1] == '1') {
      current_tile.setWeight(1);
    }
    else if (kVirtualMap[tiles_map.getCurrRow()*2 + 1][tiles_map.getCurrColumn()*2 + 1] == '2') {
      current_tile.setWeight(2);
    }
    else if (kVirtualMap[tiles_map.getCurrRow()*2 + 1][tiles_map.getCurrColumn()*2 + 1] == '3') {
      current_tile.setWeight(3);
    }
    
    //Update North
    if (kVirtualMap[tiles_map.getCurrRow()*2][tiles_map.getCurrColumn()*2 + 1] != 'W') {
      current_tile.setNorth();
      if (tiles_map.getCurrRow() > 0 && tiles_map.northTile().isVisited() == false) {
        Tile north_tile = tiles_map.northTile();
        north_tile.setSouth();
        north_tile.setAccessible();
        tiles_map.setTile(north_tile, 'N');
      }
      else if (tiles_map.getCurrRow() == 0) {
        Tile north_tile;
        north_tile.setSouth();
        north_tile.setAccessible();
        tiles_map.addRowFirst(north_tile);
      }
      
      if (kVirtualMap[tiles_map.getCurrRow()*2 - 1][tiles_map.getCurrColumn()*2 + 1] == 'B') {
        Tile north_tile = tiles_map.northTile();
        north_tile.setBlack();
        tiles_map.setTile(north_tile, 'N');
      }
    }

    //Update East
    if (kVirtualMap[tiles_map.getCurrRow()*2 + 1][tiles_map.getCurrColumn()*2 + 2] != 'W') {
      current_tile.setEast();
      if (tiles_map.getCurrColumn() < tiles_map.getColumns() - 1 && tiles_map.eastTile().isVisited() == false) { //if it is not the one on the last column
        Tile east_tile = tiles_map.eastTile();
        east_tile.setWest();
        east_tile.setAccessible();
        tiles_map.setTile(east_tile, 'E');
      }
      else if (tiles_map.getCurrColumn() == tiles_map.getColumns() - 1) {
        Tile east_tile;
        east_tile.setWest();
        east_tile.setAccessible();
        tiles_map.addColumnLast(east_tile);
      }
      
      if (kVirtualMap[tiles_map.getCurrRow()*2 + 1][tiles_map.getCurrColumn()*2 + 3] == 'B') {
        Tile east_tile = tiles_map.northTile();
        east_tile.setBlack();
        tiles_map.setTile(east_tile, 'E');
      }
    }

    //Update South
    if (kVirtualMap[tiles_map.getCurrRow()*2 + 2][tiles_map.getCurrColumn()*2 + 1] != 'W') {
      current_tile.setSouth();
      if (tiles_map.getCurrRow() < tiles_map.getRows() - 1 && tiles_map.southTile().isVisited() == false) {
        Tile south_tile = tiles_map.southTile();
        south_tile.setNorth();
        south_tile.setAccessible();
        tiles_map.setTile(south_tile, 'S');
      } 
      else if (tiles_map.getCurrRow() == tiles_map.getRows() - 1) {
        Tile south_tile;
        south_tile.setNorth();
        south_tile.setAccessible();
        tiles_map.addRowLast(south_tile);
      }
      
      if (kVirtualMap[tiles_map.getCurrRow()*2 + 3][tiles_map.getCurrColumn()*2 + 1] == 'B') {
        Tile south_tile = tiles_map.northTile(); 
        south_tile.setBlack();
        tiles_map.setTile(south_tile, 'S'); 
      }
    }

    //Update West
    if (kVirtualMap[tiles_map.getCurrRow()*2 + 1][tiles_map.getCurrColumn()*2] != 'W') {
      current_tile.setWest();
      if (tiles_map.getCurrColumn() > 0 && tiles_map.westTile().isVisited() == false)  {
        Tile west_tile = tiles_map.westTile();
        west_tile.setEast();
        west_tile.setAccessible();
        tiles_map.setTile(west_tile, 'W');
      }
      else if (tiles_map.getCurrColumn() == 0) {
        Tile west_tile;
        west_tile.setEast();
        west_tile.setAccessible();
        tiles_map.addColumnFirst(west_tile);
      }

      if (kVirtualMap[tiles_map.getCurrRow()*2 + 1][tiles_map.getCurrColumn()*2 - 1] == 'B') {
        Tile west_tile = tiles_map.northTile();
        west_tile.setBlack();
        tiles_map.setTile(west_tile, 'W');
      }
    }

    tiles_map.setTile(current_tile, '0');

  return tiles_map;
}