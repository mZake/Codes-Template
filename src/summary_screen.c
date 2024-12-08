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
