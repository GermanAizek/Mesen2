#pragma once 
#include "stdafx.h"
#include "NES/INesMemoryHandler.h"
#include "Utilities/ISerializable.h"
#include "NES/NesTypes.h"

enum class ConsoleRegion;

class Emulator;
class BaseMapper;
class ControlManager;
class NesConsole;
class EmuSettings;

class BaseNesPpu : public INesMemoryHandler, public ISerializable
{
protected:
	uint64_t _masterClock;
	uint32_t _cycle;
	int16_t _scanline;
	bool _emulatorBgEnabled;
	bool _emulatorSpritesEnabled;
	//16
	uint16_t _videoRamAddr;
	uint16_t _highBitShift;
	uint16_t _lowBitShift;
	uint16_t _backgroundPatternAddr;
	uint8_t _masterClockDivider;
	uint8_t _spriteRamAddr;
	bool _largeSprites;
	uint8_t _xScroll;
	bool _enableOamDecay;
	bool _needStateUpdate;
	bool _renderingEnabled;
	bool _prevRenderingEnabled;
	//32
	bool _sprite0Visible;
	uint8_t _spriteCount;
	uint8_t _secondaryOAMAddr;
	uint8_t _oamCopybuffer;
	bool _spriteInRange;
	bool _sprite0Added;
	uint8_t _spriteAddrH;
	uint8_t _spriteAddrL;
	uint8_t _overflowBugCounter;
	bool _oamCopyDone;
	uint16_t _minimumDrawBgCycle;
	uint16_t _minimumDrawSpriteCycle;
	uint16_t _minimumDrawSpriteStandardCycle;
	//48
	BaseMapper* _mapper;
	uint16_t* _currentOutputBuffer;
	////////////////////////
	//64 : end of cache line
	////////////////////////
	uint8_t _paletteRAM[0x20];
	uint8_t _secondarySpriteRAM[0x20];
	////////////////////////
	//128 : end of cache line
	////////////////////////
	TileInfo _tile;
	uint16_t _ppuBusAddress;
	uint16_t _nmiScanline;
	uint8_t _currentTilePalette;
	uint8_t _previousTilePalette;
	uint16_t _tmpVideoRamAddr;
	uint16_t _spritePatternAddr;
	//144
	uint32_t _spriteIndex;
	int32_t _lastUpdatedPixel;
	uint16_t _updateVramAddr;
	uint16_t _intensifyColorBits;
	uint8_t _paletteRamMask;
	uint8_t _updateVramAddrDelay;
	bool _verticalWrite; //not used in rendering
	bool _writeToggle; //not used in rendering
	//160
	NesSpriteInfo* _lastSprite; //used by HD ppu
	NesConsole* _console;
	//176
	uint32_t _frameCount;
	bool _preventVblFlag;
	bool _vBlank; //not used in rendering

	uint8_t _statusReg; //not used in rendering
	uint8_t _controlReg; //not used in rendering
	uint8_t _maskReg; //not used in rendering
	bool _grayscale; //not used in rendering
	bool _backgroundMask; //not used in rendering
	bool _spriteMask; //not used in rendering
	bool _backgroundEnabled; //not used in rendering
	bool _spritesEnabled; //not used in rendering
	bool _intensifyRed; //not used in rendering
	bool _intensifyGreen; //not used in rendering

	////////////////////////
	//192 : end of cache line
	////////////////////////
	uint8_t _spriteRAM[0x100];
	////////////////////////
	//448 : end of cache line
	////////////////////////
	bool _hasSprite[257];
	//705
	NesSpriteInfo _spriteTiles[64];

	bool _intensifyBlue; //not used in rendering

	Emulator* _emu;
	EmuSettings* _settings;
	uint16_t* _outputBuffers[2];

	ConsoleRegion _region;
	uint16_t _standardVblankEnd;
	uint16_t _standardNmiScanline;
	uint16_t _vblankEnd;
	uint16_t _palSpriteEvalScanline;

	uint8_t _memoryReadBuffer;
	PPUStatusFlags _statusFlags;

	uint8_t _firstVisibleSpriteAddr; //For extra sprites
	uint8_t _lastVisibleSpriteAddr; //For extra sprites

	uint32_t _ignoreVramRead;
	uint8_t _openBus;
	int32_t _openBusDecayStamp[8];

	uint64_t _oamDecayCycles[0x40];
	bool _corruptOamRow[32];
	
	__forceinline bool IsRenderingEnabled();

public:
	virtual void Reset() = 0;
	virtual void Run(uint64_t runTo) = 0;

	uint32_t GetFrameCount() { return _frameCount; }
	uint32_t GetCurrentCycle() { return _cycle; }
	int32_t GetCurrentScanline() { return _scanline; }
	uint32_t GetFrameCycle() { return ((_scanline + 1) * 341) + _cycle; }

	virtual uint16_t* GetScreenBuffer(bool previousBuffer) = 0;
	virtual void SetRegion(ConsoleRegion region) = 0;

	void GetState(NesPpuState& state);
	void SetState(NesPpuState& state);

	uint16_t GetCurrentBgColor();

	uint8_t ReadPaletteRam(uint16_t addr);
	void WritePaletteRam(uint16_t addr, uint8_t value);

	virtual PpuModel GetPpuModel() = 0;
	virtual uint32_t GetPixelBrightness(uint8_t x, uint8_t y) = 0;

	virtual void GetMemoryRanges(MemoryRanges& ranges) override {}
	virtual uint8_t ReadRam(uint16_t addr) override { return 0; }
	virtual void WriteRam(uint16_t addr, uint8_t value) override {}

	virtual void Serialize(Serializer& s) override {}
};
