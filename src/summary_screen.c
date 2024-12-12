#include "global.h"

// Extracts the upper 16 bits of a 32-bit number
#define HIHALF(n) (((n) & 0xFFFF0000) >> 16)

// Extracts the lower 16 bits of a 32-bit number
#define LOHALF(n) ((n) & 0xFFFF)

static void PSS_PlayMonCry(void); // PokeSum_TryPlayMonCry
static void PSS_RemoveAllWindows(u8 curPageIndex); // PokeSum_RemoveWindows
static void PSS_SetHelpContext(void); // PokeSum_SetHelpContext
static void PSS_AddTextToWin3(void); // PokeSum_PrintRightPaneText
static void PSS_AddTextToWin4(void); // PokeSum_PrintBottomPaneText
static void PSS_AddTextToWin5(void); // PokeSum_PrintAbilityDataOrMoveTypes
static void PSS_DrawMonMoveIcon(void); // PokeSum_PrintMonTypeIcons
static void PSS_AddTextToWin0(const u8 * str); // PokeSum_PrintPageName
static void PSS_AddTextToWin1(const u8 * str); // PokeSum_PrintControlsString
static void PSS_AddTextToWin2(const u8 * str); // PrintMonLevelNickOnWindow2
static void PSS_SetInvisibleHpBar(u8 invisible); // ShowOrHideHpBarObjs
static void PSS_SetInvisibleExpBar(u8 invisible); // ShowOrHideExpBarObjs
static void PSS_SetInvisibleMonSprite(u8 invisible); // PokeSum_ShowOrHideMonPicSprite
static void PSS_SetInvisibleMarkings(u8 invisible); // PokeSum_ShowOrHideMonMarkingsSprite
static void PSS_SetInvisiblePokeball(u8 invisible); // ShowOrHideBallIconObj
static void PSS_SetInvisibleIconStatus(u8 invisible); // ShowOrHideStatusIcon
static void PSS_TurnOffScreen(void); // PokeSum_Setup_ResetCallbacks
static void PSS_InitDisplay(void); // PokeSum_Setup_InitGpu
static void PSS_ResetAll(void); // PokeSum_Setup_SpritesReset
static u8 PSS_LoadTilesAndMap(void); // PokeSum_HandleLoadBgGfx
static u8 PSS_LoadSpritesData(void); // PokeSum_HandleCreateSprites
static void PSS_InitWindows(void); // PokeSum_CreateWindows
static void PSS_GetDataPokemon(void); // BufferMonInfo
static void PSS_GetStatsPokemon(void); // BufferMonSkills
static u8 PSS_CheckMonStatus(u32 status); // StatusToAilment
static void PSS_GetMoveNames(u8); // BufferMonMoveI
static u16 PSS_GetMonMove(struct Pokemon * mon, u8 moveSlot); // GetMonMoveBySlotId
static u16 PSS_GetMovePP(struct Pokemon * mon, u8 moveSlot); // GetMonPpByMoveSlot
static void PSS_LoadShinyIndicator(u16, u16); // CreateShinyStarObj
static void PSS_LoadUnkPoint(u16, u16); // CreatePokerusIconObj
static void PSS_LoadMarkings(void); // PokeSum_CreateMonMarkingsSprite
static void PSS_LoadMovesSelectCursor(u16, u16); // CreateMoveSelectionCursorObjs
static void PSS_LoadIconStatus(u16, u16); // CreateMonStatusIconObj
static void PSS_LoadHpBar(u16, u16); // CreateHpBarObjs
static void PSS_LoadExpBar(u16, u16); // CreateExpBarObjs
static void PSS_LoadPokeball(void); // CreateBallIconObj
static void PSS_LoadMonIcon(void); // PokeSum_CreateMonIconSprite
static void PSS_LoadMonSprite(void); // PokeSum_CreateMonPicSprite
static void PSS_ShowInfoPokemon(void); // PrintInfoPage
static void PSS_ShowMonStats(void); // PrintSkillsPage
static void PSS_PrintMoveNamesOrCancel(void); // PrintMovesPage
static void PSS_PrintMoveNamesAndPP(u8 i); // PokeSum_PrintMoveName
static void PSS_CheckIfMonIsEgg(void); // PokeSum_PrintTrainerMemo
static void PSS_PrintExpPointAndNextLvTexts(void); // PokeSum_PrintExpPoints_NextLv
static void PSS_ShowAttackInfo(void); // PokeSum_PrintSelectedMoveStats
static void PSS_ShowEggInfo(void); // PokeSum_PrintTrainerMemo_Mon
static void PSS_ShowMonInfo(void); // PokeSum_PrintTrainerMemo_Egg
static bool32 PSS_IsNatureBoldOrGentle(u8 nature); // PokeSum_IsMonBoldOrGentle
static void PSS_ShowTrainerMemo(void); // PokeSum_PrintTrainerMemo_Mon_NotHeldByOT
static bool32 PSS_IsMonFromGenIII(void); // CurrentMonIsFromGBA
static void PSS_PrintAbilityNameAndDescription(void); // PokeSum_PrintAbilityNameAndDesc
static void PSS_DrawMoveIcon(void); // PokeSum_DrawMoveTypeIcons
static void PSS_ShowIconStatus(void); // UpdateMonStatusIconObj
static void PSS_SetupHpBar(void); // UpdateHpBarObjs
static void PSS_SetupExpBar(void); // UpdateExpBarObjs
static void PSS_UnloadMarkings(void); // PokeSum_DestroyMonMarkingsSprite
static void PSS_GetMarkings(void); // PokeSum_UpdateMonMarkingsAnim
static void PSS_ScrollPSSBackground(void);

struct PokemonSummaryScreenData
{
    u8 window[7];

    u8 spriteId_0;
    u8 spriteId_1;
    u8 spriteId_2;

    u8 task;
    u8 unk301C;

    u8 unk3020;

    bool32 isEnemyParty; /* 0x3024 */

    struct PokeSummary
    {
        u8 nickname[POKEMON_NAME_LENGTH + 1];
        u8 specieName[POKEMON_NAME_LENGTH + 1];
        u8 ot_name[12];
        u8 unk304C[2][12];

        u8 dexNum[5];
        u8 ot_id[7];
        u8 heldItem[ITEM_NAME_LENGTH + 1];

        u8 genderSymbol[3];
        u8 level[7];
        u8 unk3090[9];
        u8 unk309C[5][5];

        u8 unk30B8[5][11];
        u8 unk30F0[5][11];
        u8 moveName[5][MOVE_NAME_LENGTH + 1];
        u8 power[5][5];
        u8 unk3188[5][5];

        u8 unk31A4[9];
        u8 unk31B0[9];

        u8 abilityName[13];
        u8 abilityDescription[52];
    } summary;

    u8 isEgg; /* 0x3200 */
    u8 isBadEgg; /* 0x3204 */

    u8 mode; /* 0x3208 */
    u8 unk320C; /* 0x320C */
    u8 lastIndex; /* 0x3210 */
    u8 curPageIndex; /* 0x3214 */
    u8 unk3218; /* 0x3218 */
    u8 isBoxMon; /* 0x321C */
    u8 typeIcons[2]; /* 0x3220 */

    u8 unk3224; /* 0x3224 */
    u8 unk3228; /* 0x3228 */
    u8 unk322C; /* 0x322C */
    u8 unk3230; /* 0x3230 */

    u8 lockMovesFlag; /* 0x3234 */

    u8 unk3238; /* 0x3238 */
    u8 unk323C; /* 0x323C */
    u8 unk3240; /* 0x3240 */
    u8 unk3244; /* 0x3244 */
    u8 unk3248; /* 0x3248 */
    s16 unk324C; /* 0x324C */

    u16 move[5]; /* 0x3250 */
    u16 currentMove[5]; /* 0x325A */
    u8 unk3264; /* 0x3264 */
    u8 unk3268; /* 0x3268 */

    u8 monStatus; /* 0x326C */

    u8 state3270; /* 0x3270 */
    u8 state3274; /* 0x3274 */
    u8 unk3278; /* 0x3278 */
    u8 state; /* 0x327C */
    u8 unk3280; /* 0x3280 */
    u8 unk3284; /* 0x3284 */
    u8 unk3288; /* 0x3288 */
    u8 unk328C; /* 0x328C */

    struct Pokemon currentMon; /* 0x3290 */

    union
    {
        struct Pokemon * mons;
        struct BoxPokemon * boxMons;
    } monList;

    MainCallback savedCallback;
    struct Sprite * markingSprite;

    u8 unk3300[2]; /* 0x3300 */
    u8 unk3304[3]; /* 0x3304 */
    u8 unused[200]; /* 0x3304 */
};

struct Struct203B144
{
    u16 unk00;
    u16 unk02;
    u16 tileTag;
    u16 palTag;
    u16 unk08;
    u16 unk0A;
    u16 unk0C;
    u16 unk0E;
    u16 unk10;

    u16 unk12[5];
    u16 unk1C[5];

    u16 unk26;
};

struct Struct203B160
{
    struct Sprite * sprites[11]; /* 0x00 */
    u16 cordX[11]; /* 0x2c */
    u16 tileTag; /* 0x42 */
    u16 palTag; /* 0x44 */
};

struct Struct203B15C
{
    struct Sprite * sprites[10]; /* 0x00 */
    u16 cordX[10]; /* 0x28 */
    u16 unk3C; /* 0x3c */
    u16 unk3E; /* 0x3e */
};

struct Struct203B170
{
    u8 unk00; /* 0x00 */
    u8 tileTag; /* 0x04 */
    u8 unk08; /* 0x08 */
};

struct Struct203B148
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
    u16 unk08; /* 0x08 */
};

struct Struct203B158
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

struct Struct203B164
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

struct Struct203B168
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

extern struct PokemonSummaryScreenData *sMonSummaryScreen;
extern struct Struct203B144 *sUnknown_203B144;
extern struct Struct203B148 *sUnknown_203B148[4];
extern struct Struct203B158 *sStatusIconSummaryScreen;
extern struct Struct203B15C *sHpBarSummaryScreen;
extern struct Struct203B160 *sExpBarSummaryScreen;
extern struct Struct203B164 *sUnknown_203B164;
extern struct Struct203B168 *sUnknown_203B168;
extern struct Struct203B170 *sUnknown_203B170;
extern u8 sLastViewedMonIndex;
extern u8 sUnknown_203B16D;
extern u8 sUnknown_203B16E;
