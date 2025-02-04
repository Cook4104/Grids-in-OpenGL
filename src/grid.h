#pragma once

#include <unordered_map>

struct GridChunk {
	unsigned int Tiles[16][16] = { 0 };
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

class Grid {
private:
	std::unordered_map<ChunkCoordinate,GridChunk,ChunkCoordinateHasher> Chunks;
public:
	Grid(int startRadius = 5);
	void SetIndexAt(int x, int y,int index);
	unsigned int GetIndexAt(int x, int y);
	GridChunk GetChunk(ChunkCoordinate coordinate);
};