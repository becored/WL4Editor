#include "EntitySet.h"

namespace LevelComponents
{
void EntitySet::LoadSubPalettes(int startPaletteId, int paletteNum, int paletteSetPtr)
{
    for(int i = 0; i < paletteNum; ++i)
    {
        // First color is transparent
        palettes[i + startPaletteId].push_back(0);
        int subPalettePtr = paletteSetPtr + i * 32;
        for(int j = 1; j < 16; ++j)
        {
            unsigned short color555 = *(unsigned short*) (ROMUtils::CurrentFile + subPalettePtr + j * 2);
            int r = ((color555 << 3) & 0xF8) | ((color555 >> 2) & 3);
            int g = ((color555 >> 2) & 0xF8) | ((color555 >> 7) & 3);
            int b = ((color555 >> 7) & 0xF8) | ((color555 >> 13) & 3);
            int a = 0xFF;
            palettes[i + startPaletteId].push_back(QColor(r, g, b, a).rgba());
        }
    }
}

void EntitySet::LoadSpritesTiles(int tileaddress, int datalength, int startrow)
{
    for(int i = 0; i < (datalength / 32); ++i)
    {
        tile8x8data[i + startrow * 32] = new Tile8x8(tileaddress + i * 32, palettes);
    }
}

EntitySet::EntitySet(int _EntitySetID, int basicElementPalettePtr)
    {
        this->EntitySetID = _EntitySetID;
        int entitysetptr = ROMUtils::PointerFromData(WL4Constants::EntitySetInfoPointerTable + _EntitySetID * 4);

        // Load 16 color palettes, ignore the first 3 rows, they are only for wario tiles
        int palettePtr;
        int tmpEntityId;
        int EntityPaletteNum;
        int k = 0;
        int currentpaletteID = 0;
        do  // Load palette 8 - 14 if exist for entities
        {
            tmpEntityId = (int) ROMUtils::CurrentFile[entitysetptr + 2 * k];
            EntityPaletteNum = (int) ROMUtils::CurrentFile[entitysetptr + 2 * k + 1];
            if((tmpEntityId > 0x10) && (EntityPaletteNum != currentpaletteID))
            {
                palettePtr = ROMUtils::PointerFromData(WL4Constants::EntityPalettePointerTable + 4 * (tmpEntityId - 0x10));
                LoadSubPalettes(8, EntityPaletteNum - currentpaletteID, palettePtr);
                currentpaletteID = EntityPaletteNum;
            }
            k++;
        }while((tmpEntityId != 0) && (currentpaletteID != 7));
        if(currentpaletteID < 7) // Set palette before and not include 15 to be 0 if not exist
        {
            for(int i = (8 + currentpaletteID); i < 15; ++i)
            {
                for(int j = 1; j < 16; ++j)
                {
                    palettes[i].push_back(0);
                }
            }
        }
        LoadSubPalettes(3, 5, basicElementPalettePtr); // Load palette 3 - 7 for Basic Element used in the room
        LoadSubPalettes(15, 1, ROMUtils::PointerFromData(WL4Constants::EntityPalettePointerTable)); // Load palette 15 for treasure boxes
        for(int i = 0; i < 2; ++i) // Set palette 0 - 2 all to 0 for Wario Sprites only
        {
            for(int j = 1; j < 16; ++j)
            {
                palettes[i].push_back(0);
            }
        }

        // Load 1024 sprites tiles, ignore first 4 rows, they are wario tiles
        Tile8x8 *blankTile = Tile8x8::CreateBlankTile(palettes);
        for(int i = 0; i < (4 * 32); ++i)
        {
            tile8x8data[i] = blankTile;
        }
        int tiledataptr, tiledatalength;
        tiledataptr = WL4Constants::SpritesBasicElementTiles; tiledatalength = 0x3000;
        LoadSpritesTiles(tiledataptr, tiledatalength, 4);
        k = 0; int currentrow = 16;
        do
        {
            tmpEntityId = (int) ROMUtils::CurrentFile[entitysetptr + 2 * k];
            if(tmpEntityId == 0) break;
            tiledataptr = ROMUtils::PointerFromData(WL4Constants::EntityTilesetPointerTable + 4 * (tmpEntityId - 0x10));
            tiledatalength = ROMUtils::IntFromData(WL4Constants::EntityTilesetLengthTable + 4 * (tmpEntityId - 0x10));
            LoadSpritesTiles(tiledataptr, tiledatalength, currentrow);
            currentrow += tiledatalength / (32 * 32); k++;
        }while(1);
        if(currentrow < 30)
        {
            for(int i = currentrow * 32; i < (30 * 32); ++i)
            {
                tile8x8data[i] = blankTile;
            }
        }
        tiledataptr = ROMUtils::PointerFromData(WL4Constants::EntityTilesetPointerTable);
        tiledatalength = ROMUtils::IntFromData(WL4Constants::EntityTilesetLengthTable);
        LoadSpritesTiles(tiledataptr, tiledatalength, 30);

        // TODO
    }
}