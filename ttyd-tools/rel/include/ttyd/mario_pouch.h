#pragma once

namespace ttyd::mario_pouch {
	
extern "C" {

// pouchGetYoshiName
// pouchSetYoshiName
// pouchSetPartyColor
// pouchGetPartyColor
// pouchCheckMail
// pouchOpenMail
// pouchReceiveMail
// pouchReceiveMailCount
// pouchGetStarStone
// pouchAddKpaScore
// pouchGetKpaScore
// pouchAddKpaCoin
// pouchGetKpaCoin
// pouchMajinaiInit
// pouchArriveBadge
// unk_JP_US_PAL_049_800cf0e8
// pouchEquipBadgeID
// pouchEquipCheckBadgeIndex
// pouchEquipCheckBadge
// pouchUnEquipBadgeIndex
// pouchEquipBadgeIndex
// pouchGetStarPoint
// pouchRevisePartyParam
void pouchReviseMarioParam();
// pouchRemoveKeepItem
// pouchAddKeepItem
// pouchGetPartyAttackLv
// pouchGetHammerLv
// pouchGetJumpLv
// pouchSetAudienceNum
// pouchGetAudienceNum
// pouchGetMaxAP
// pouchSetAP
// pouchAddAP
// pouchGetAP
// pouchSetMaxFP
// pouchSetFP
// pouchGetMaxFP
// pouchGetFP
// pouchSetPartyHP
// pouchGetPartyHP
// pouchSetMaxHP
// pouchSetHP
// pouchGetMaxHP
// pouchGetHP
// pouchAddHP
// pouchAddStarPiece
// pouchGetStarPiece
// pouchSetSuperCoin
// pouchGetSuperCoin
// pouchSetCoin
// pouchAddCoin
// pouchGetCoin
// pouchSortItem
// comp_kind_r
// comp_aiueo_r
// comp_kind
// comp_aiueo
// unk_JP_US_PAL_050_800d06b4
// pouchRemoveItemIndex
// pouchRemoveItem
// pouchCheckItem
// pouchGetItem
// pouchGetEmptyKeepItemCnt
// pouchGetEmptyHaveItemCnt
// pouchGetEquipBadgeCnt
// pouchGetHaveBadgeCnt
// pouchGetKeepItemCnt
// pouchGetHaveItemCnt
// pouchHaveBadge
// pouchKeepItem
// pouchHaveItem
// pouchKeyItem
// pouchInit
uint32_t pouchGetPtr();

}

}