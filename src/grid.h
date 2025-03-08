#pragma once

#include <unordered_map>
#include <assert.h>

template<typename T>
struct GridChunk {
	T Tiles[16][16] = { 0 };
};

struct ChunkCoordinate {
	int x, y; // origin is at the bottom left tile
	bool operator==(const ChunkCoordinate& c) const{
		return x == c.x && y == c.y;
	}
};

struct ChunkCoordinateHasher {
	std::size_t operator()(const ChunkCoordinate& c) const {
		return std::hash<int>()(c.x) ^ std::hash<int>()(c.y);
	}
};

template<typename T>
class Grid {
private:
	std::unordered_map<ChunkCoordinate,GridChunk<T>,ChunkCoordinateHasher> Chunks;
public:
	Grid(int startRadius = 5) {
		for (int y = -startRadius; y < startRadius; y++) { // create starting chunks from a diameter
			for (int x = -startRadius; x < startRadius; x++) {
				GridChunk<T> newChunk;
				ChunkCoordinate chunkCoord;
				chunkCoord.x = x;
				chunkCoord.y = y;
				Chunks.insert(std::make_pair(chunkCoord, newChunk));
			}
		}
	}
	void SetElementAt(int x, int y, T element) {
		ChunkCoordinate calcCoord = { (int)floorf((float)x / 16.0f),(int)floorf((float)y / 16.0f) };
		if (Chunks.find(calcCoord) != Chunks.end()) {
			GridChunk<T>& foundChunk = Chunks[calcCoord];
			int tileX = (x % 16 + 16) % 16;
			int tileY = (y % 16 + 16) % 16;
			foundChunk.Tiles[tileY][tileX] = element;
			return;
		}
		assert(0 && "Coordinate out of bounds");

	}
	T* GetElementAt(int x, int y) {
		ChunkCoordinate calcCoord = { (int)floorf((float)x / 16.0f),(int)floorf((float)y / 16.0f) };
		if (Chunks.find(calcCoord) != Chunks.end()) {
			GridChunk foundChunk = Chunks[calcCoord];
			int tileX = (x % 16) + (std::signbit(x) * 16);
			int tileY = (y % 16) + (std::signbit(y) * 16);
			return &foundChunk.Tiles[tileY][tileX];
		}
		assert(0 && "Coordinate out of bounds");
		return nullptr;
	}
	GridChunk<T> GetChunk(ChunkCoordinate coordinate) {
		assert(Chunks.find(coordinate) != Chunks.end() && "Coordinate out of bounds");
		return Chunks[coordinate];
	}
};