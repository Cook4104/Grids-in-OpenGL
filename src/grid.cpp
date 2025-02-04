#include "grid.h"
#include <assert.h>

Grid::Grid(int startRadius) {
	for (int y = -startRadius; y < startRadius; y++) { // create starting chunks from a diameter
		for (int x = -startRadius; x < startRadius; x++) {
			GridChunk newChunk;
			ChunkCoordinate chunkCoord;
			chunkCoord.x = x;
			chunkCoord.y = y;
			Chunks.insert(std::make_pair(chunkCoord, newChunk));
		}
	}
}

void Grid::SetIndexAt(int x, int y,int index) {
	ChunkCoordinate calcCoord = { (int)floorf((float)x / 16.0f),(int)floorf((float)y / 16.0f) };
	if (Chunks.find(calcCoord) != Chunks.end()) {
		GridChunk& foundChunk = Chunks[calcCoord];
		int tileX = (x % 16) + (signbit(x) * 16);
		int tileY = (y % 16) + (signbit(y) * 16);
		foundChunk.Tiles[tileY][tileX] = index;
		return;
	}
	assert(0 && "Coordinate out of bounds");
}

unsigned int Grid::GetIndexAt(int x, int y) {
	ChunkCoordinate calcCoord = { (int)floorf((float)x / 16.0f),(int)floorf((float)y / 16.0f) };
	if (Chunks.find(calcCoord) != Chunks.end()) {
		GridChunk foundChunk = Chunks[calcCoord];
		int tileX = (x % 16) + (signbit(x) * 16);
		int tileY = (y % 16) + (signbit(y) * 16);
		return foundChunk.Tiles[tileY][tileX];
	}
	assert(0 && "Coordinate out of bounds");
	return 0;
}

GridChunk Grid::GetChunk(ChunkCoordinate coord) {
	assert(Chunks.find(coord) != Chunks.end() && "Coordinate out of bounds");
	return Chunks[coord];
}