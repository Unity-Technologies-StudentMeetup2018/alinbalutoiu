#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include<algorithm>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <set>

struct vector2
{
public:
	int32_t x, y;

	bool operator==(const vector2& other)
	{
		return x == other.x && y == other.y;
	}

	vector2() {
		x = 0;
		y = 0;
	}

	vector2(int32_t x_new, int32_t y_new) {
		x = x_new;
		y = y_new;
	}

	vector2 operator+(const vector2& other)
	{
		return vector2(x+other.x, y+other.y);
	}
};

struct Node
{
	int32_t G, H;
	vector2 coordinates;
	Node *parent;

	Node(vector2 coord_, Node *parent_ = nullptr);
	int32_t getScore() { return G + H; };
};

struct Map
{

public:
	using NodeSet = std::set<Node*>;
	static const char cellBlocked = '#';
	static const char cellFree = '.';
	static const char cellInvalid = '?';
	static const char cellPath = '@';

	static const uint32_t kMapSize = 32;

	std::vector<vector2> direction = {
		{ 0, 1 },{ 1, 0 },{ 0, -1 },{ -1, 0 },
		{ -1, -1 },{ 1, 1 },{ -1, 1 },{ 1, -1 }
	};

	Map(const std::vector<char>& mapFileData, vector2 startPoint, vector2 endPoint)
	{
		uint32_t idx = 0;

		for (int32_t i = 0; i < mapFileData.size() && idx < kMapSize*kMapSize; ++i)
		{
			if (mapFileData[i] == cellBlocked || mapFileData[i] == cellFree)
			{
				m_MapData[idx%kMapSize][idx / kMapSize] = mapFileData[i];
				idx++;
			}
			// Skipping new line and eventual spaces at the end of line.
		}

		m_StartPosition = startPoint;
		m_GoalPosition = endPoint;
	}

	std::string SolutionToString()
	{
		std::string sol;
		sol.reserve(kMapSize*kMapSize);

		for (int32_t i = 0; i < kMapSize; ++i)
		{
			sol.append(m_MapData[i], kMapSize);
			sol.append("\n");
		}
		return sol;
	}

	void DisplayMap()
	{
		for (int32_t y = 0; y < kMapSize; ++y)
		{
			for (int32_t x = 0; x < kMapSize; ++x)
			{
				if (x == m_StartPosition.x && y == m_StartPosition.y)
					putchar(cellPath);
				else if (x == m_GoalPosition.x && y == m_GoalPosition.y)
					putchar(cellPath);
				else
				{
					switch (m_MapData[x][y])
					{
					case cellBlocked: putchar(cellBlocked); break;
					case cellFree: putchar(cellFree); break;
					case cellPath: putchar(cellPath); break;
					default: putchar(cellInvalid); break;
					}
				}
			}
			putchar('\n');
		}
	}

	// Trying to implement A*
	bool ComputePath()
	{
		// following pseudocode from http://www.growingwiththeweb.com/2012/06/a-pathfinding-algorithm.html
		NodeSet openSet, closedSet;
		Node* start = new Node(m_StartPosition);
		openSet.insert(start);
		start->G = 0;
		start->H = start->G + heuristicDiagonal(start->coordinates);
		Node *current = nullptr;
		while (!openSet.empty()) {
			// finding set with lowest score
			current = *openSet.begin();
			for (auto node : openSet) {
				if (node->getScore() <= current->getScore()) {
					current = node;
				}
			}
			if (current->coordinates == m_GoalPosition) {
				break;
			}
			// remove current node from openset list
			openSet.erase(std::find(openSet.begin(), openSet.end(), current));
			// add current to closed_list
			closedSet.insert(current);
			// 8 way movement, finding neighbours
			for (int i = 0; i < 8; i++) {
				vector2 newPosition(current->coordinates + direction[i]);

			}
		}
		while (current != nullptr) {
			m_MapData[current->coordinates.x][current->coordinates.y] = cellPath;
			current = current->parent;
		}
		return true;
	}
	
	int heuristicDiagonal(vector2 position) {
		return std::max(std::abs(position.x-m_GoalPosition.x),
			std::abs(position.y- m_GoalPosition.y));
	}

	bool detectColision(vector2 position) {
		if (m_MapData[position.x][position.y] == cellFree)
			return true;
		return false;
	}
	
	/* DUMB ALGORITHM

	bool ComputePath()
	{
	bool bla = FindPath(m_StartPosition, m_MapData);
	return true;
	}
	bool SafeChoice(vector2 desiredPosition, char m_MapDataClone[][kMapSize]) {
		if (m_MapDataClone[desiredPosition.x][desiredPosition.y] == cellFree)
			return true;
		return false;
	}

	void FreePosition(vector2 position, char m_MapData[][kMapSize]) {
		m_MapData[position.x][position.y] = cellFree;
	}

	void occupyPosition(vector2 position, char m_MapData[][kMapSize]) {
		m_MapData[position.x][position.y] = cellPath;
	}

	bool FindPath(vector2 currentPosition, char m_MapDataClone[][kMapSize]) {
		//DisplayMap();
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (currentPosition.y == m_GoalPosition.y && currentPosition.x == m_GoalPosition.x) {
			return true;
		}
		occupyPosition(currentPosition, m_MapDataClone);
		currentPosition.x++;
		if (SafeChoice(currentPosition, m_MapDataClone) == true) {
			if (FindPath(currentPosition, m_MapDataClone))
				return true;
		}
		currentPosition.x--;
		currentPosition.x--;
		if (SafeChoice(currentPosition, m_MapDataClone) == true)
			if (FindPath(currentPosition, m_MapDataClone))
				return true;
		currentPosition.x++;
		currentPosition.y++;
		if (SafeChoice(currentPosition, m_MapDataClone) == true)
			if (FindPath(currentPosition, m_MapDataClone))
				return true;
		currentPosition.y--;
		currentPosition.y--;
		if (SafeChoice(currentPosition, m_MapDataClone) == true)
			if (FindPath(currentPosition, m_MapDataClone))
				return true;
		currentPosition.y++;
		FreePosition(currentPosition, m_MapDataClone);
		return false;
	}
	*/

private:

	char m_MapData[kMapSize][kMapSize];
	vector2 m_StartPosition;
	vector2 m_GoalPosition;
};