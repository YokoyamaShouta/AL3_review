#include "MapChipField.h"
#include <map>
#include <fstream>
#include <sstream>
#include<assert.h>

using namespace KamataEngine;
using namespace std;

namespace
{
    map<string, MapChipField::MapChipType> mapChipTable = 
        {
            {"0", MapChipField::MapChipType::BLANK},
            {"1", MapChipField::MapChipType::BLOCK},
            {"2", MapChipField::MapChipType::TRAP},
            {"3", MapChipField::MapChipType::COIN},
        };
}

void MapChipField::ResetMapChipDate()
{
	mapChipData_.date.clear();
	mapChipData_.date.resize(numBlockVirtical);
    for (vector<MapChipType>& mapChipDateLine : mapChipData_.date)
    {
		mapChipDateLine.resize(numBlockHorizontal);
    }
}

void MapChipField::LoadmapChipCsv(const std::string& filePath) 
{
    MapChipField::ResetMapChipDate(); 

    ifstream file;
	file.open(filePath);
	assert(file.is_open());

    stringstream mapChipCsv;
	mapChipCsv << file.rdbuf();
	file.close();

    for (uint32_t i = 0; i < numBlockVirtical; i++)
    {
		string line;
		getline(mapChipCsv, line);

        istringstream line_stream(line);

        for (uint32_t j = 0; j < numBlockHorizontal; j++)
        {
			string word;
			getline(line_stream, word, ',');

            if (mapChipTable.contains(word))
            {
				mapChipData_.date[i][j] = mapChipTable[word];
            }
        }
    }
}

void MapChipField::SetMapChipTypeByIndex(uint32_t indexX, uint32_t indexY, MapChipType mapChip) 
{
	mapChipData_.date[indexY][indexX] = mapChip; 
}

MapChipField::MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex)
{ 
    if (xIndex < 0 || numBlockHorizontal - 1 < xIndex) {
		return MapChipType::BLANK;
	}

	if (yIndex < 0 || numBlockVirtical - 1 < yIndex) {
		return MapChipType::BLANK;
	}

	return mapChipData_.date[yIndex][xIndex];
}
              
Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) 
{
    return Vector3(blockWidth * xIndex, blockHeight * (numBlockVirtical - 1 - yIndex), 0); 
}

uint32_t MapChipField::GetNumBlockVirtical() { return numBlockVirtical; }

uint32_t MapChipField::GetNumBlockHorizontal() { return numBlockHorizontal; }

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const KamataEngine::Vector3& positon)
{
	IndexSet indexSet = {};
	indexSet.indexX = uint32_t((positon.x + blockWidth / 2.0f) / blockWidth);
	indexSet.indexY = uint32_t(numBlockVirtical - 1 - ((positon.y - blockHeight / 2.0f) / blockHeight));
	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t indexX, uint32_t indexY) 
{
	// 指定ブロックの中心座標の取得
	Vector3 center = GetMapChipPositionByIndex(indexX, indexY);

	Rect rect;
	rect.left = center.x - blockWidth / 2.0f;
	rect.right = center.x + blockWidth / 2.0f;
	rect.bottom = center.y - blockHeight / 2.0f;
	rect.top = center.y + blockHeight / 2.0f;

	return rect;
}
