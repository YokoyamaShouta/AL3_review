#pragma once
#include"KamataEngine.h"

class MapChipField
{
public:
	enum class MapChipType 
	{
		BLANK,
		BLOCK,
		TRAP,
		COIN,
	};
	
	MapChipType mapChipType;

	struct mapChipDate 
	{
		std::vector<std::vector<MapChipType>> date;
	};

	struct IndexSet 
	{
		uint32_t indexX, indexY;
	};

	struct Rect
	{
		float left, right, bottom, top;
	};

	void ResetMapChipDate();

	void LoadmapChipCsv(const std::string& filePath);

	void SetMapChipTypeByIndex(uint32_t indexX, uint32_t indexY, MapChipType mapCihpType);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical();
	uint32_t GetNumBlockHorizontal();

	IndexSet GetMapChipIndexSetByPosition(const KamataEngine::Vector3& position);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	mapChipDate mapChipData_;

	float blockWidth = 1.0f;
	float blockHeight = 1.0f;

	uint32_t numBlockVirtical = 20;
	uint32_t numBlockHorizontal = 40;
};