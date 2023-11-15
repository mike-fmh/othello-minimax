
#include <cmath>
#include "Board.hpp"

using namespace othello;


std::vector<std::vector<std::shared_ptr<Tile>>> Board::allBoardTiles;


Board::Board(int boardMinWidth, int boardMaxWidth, int boardMinHeight, int boardMaxHeight, int boardPadding, RGBColor tileColor)
    :   Y_MIN_(boardMinHeight - boardPadding),
        Y_MAX_(boardMaxHeight + boardPadding),
        X_MIN_(boardMinWidth - boardPadding),
        X_MAX_(boardMaxWidth + boardPadding),
        HEIGHT_((boardMaxHeight + boardPadding) - (boardMinHeight - boardPadding)),
        WIDTH_((boardMaxWidth + boardPadding) - (boardMinWidth - boardPadding)),
        COLS_MIN_(1),
        COLS_MAX_(boardMaxWidth),
        ROWS_MIN_(1),
        ROWS_MAX_(boardMaxHeight),
        PADDING_(boardPadding),
        DEFAULT_TILE_COLOR_(tileColor)
{
    TilePoint thisPnt;
    for (int c = 1; c <= 8; c++) {
        allBoardTiles.push_back(std::vector<std::shared_ptr<Tile>>());
        for (int r = 1; r <= 8; r++) {
            thisPnt = TilePoint{r, c};
            std::shared_ptr<Tile> thisTile = std::make_shared<Tile>(thisPnt, DEFAULT_TILE_COLOR_.red, DEFAULT_TILE_COLOR_.blue, DEFAULT_TILE_COLOR_.green);
            allBoardTiles.at(c-1).push_back(thisTile);
        }
    }
}

void Board::setScalingRatios(int& paneWidth, int& paneHeight){
	float widthRatio = WIDTH_ / paneWidth;
	float heightRatio = HEIGHT_ / paneHeight;
	float maxRatio = fmax(widthRatio,heightRatio);

	pixelToWorldRatio = maxRatio;
	worldToPixelRatio = 1.f / pixelToWorldRatio;
	drawInPixelScale = pixelToWorldRatio;
	
	paneWidth = static_cast<int>(round(WIDTH_ * worldToPixelRatio));
	paneHeight = static_cast<int>(round(HEIGHT_ * worldToPixelRatio));
}

TilePoint Board::pixelToWorld(float ix, float iy)
{
	return TilePoint{	(int)round(Board::X_MIN_ + ix*Board::pixelToWorldRatio),
					(int)round(Board::Y_MAX_ - iy*Board::pixelToWorldRatio)
				};
}

TilePoint Board::pixelToWorld(const PixelPoint& pt)
{
	return TilePoint{	(int)round(Board::X_MIN_ + pt.x*Board::pixelToWorldRatio),
					(int)round(Board::Y_MAX_ - pt.y*Board::pixelToWorldRatio)
				};
}

PixelPoint Board::worldToPixel(float wx, float wy)
{
	return PixelPoint{(wx - Board::X_MIN_) * Board::worldToPixelRatio,
				 (Board::Y_MAX_ - wy) * Board::worldToPixelRatio};

}

PixelPoint Board::worldToPixel(const TilePoint& pt)
{
	return PixelPoint{(pt.x - Board::X_MIN_) * Board::worldToPixelRatio,
				 (Board::Y_MAX_ - pt.y) * Board::worldToPixelRatio};

}

std::shared_ptr<Tile> Board::getBoardTile(TilePoint& at) {
    float row = at.x;
    float col = at.y;
    if (row > Board::ROWS_MAX_)
        row = Board::ROWS_MAX_;
    if (col > Board::COLS_MAX_)
        col = Board::COLS_MAX_;
    
    // TilePoint coords go from 1-8, while boardTile->at() will range from 0-7
    // so in the boardTiles vector, all tile locations are -1 compared to them represented by TilePoints
    return allBoardTiles.at(col - 1).at(row - 1);
}

void Board::getNeighbors(TilePoint& tile, std::vector<std::shared_ptr<Tile>>* neighbors) {
    TilePoint tileLoc;
    if (tile.getCol() > 1) { // west
        tileLoc = TilePoint{tile.x - 1, tile.y};
        neighbors->push_back(getBoardTile(tileLoc));
        if (tile.getRow() > 1) { // southwest
            tileLoc = TilePoint{tile.x - 1, tile.y - 1};
            neighbors->push_back(getBoardTile(tileLoc));
        }
        if (tile.getRow() < Board::ROWS_MAX_) { // northwest
            tileLoc = TilePoint{tile.x - 1, tile.y + 1};
            neighbors->push_back(getBoardTile(tileLoc));
        }
    }
    if (tile.getCol() < Board::Y_MAX_  - 1) { // east
        tileLoc = TilePoint{tile.x + 1, tile.y};
        neighbors->push_back(getBoardTile(tileLoc));
        if (tile.getRow() > 1) { // southeast
            tileLoc = TilePoint{tile.x + 1, tile.y - 1};
            neighbors->push_back(getBoardTile(tileLoc));
        }
        if (tile.getRow() < Board::ROWS_MAX_) { // northeast
            tileLoc = TilePoint{tile.x + 1, tile.y + 1};
            neighbors->push_back(getBoardTile(tileLoc));
        }
    }
    if (tile.getRow() > 1) { // south
        tileLoc = TilePoint{tile.x, tile.y - 1};
        neighbors->push_back(getBoardTile(tileLoc));
    }
    if (tile.getRow() < Board::X_MAX_ - 1) { // north
        tileLoc = TilePoint{tile.x, tile.y + 1};
        neighbors->push_back(getBoardTile(tileLoc));
    }
}
