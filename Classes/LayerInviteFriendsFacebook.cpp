//
//  LayerInviteFriendsFacebook.cpp
//  iCasinov2
//
//  Created by AlexDao on 6/6/14.
//
//

#include "LayerInviteFriendsFacebook.h"
#include "LayerInviteFriendsFacebook.h"
#include "Requests/ExtensionRequest.h"
#include "mUtils.h"
using namespace cocos2d;
//using namespace CocosDenshion;


LayerInviteFriendsFacebook::LayerInviteFriendsFacebook()
{
    nodeListAvatar = NULL;
    //
    GameServer::getSingleton().addListeners(this);
}

LayerInviteFriendsFacebook::~LayerInviteFriendsFacebook()
{
    GameServer::getSingleton().removeListeners(this);
    //
    CC_SAFE_RELEASE(nodeListAvatar);
}

void LayerInviteFriendsFacebook::loadAllMyDatas(){
    tblAvatar->reloadData();
    //Send request
//    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
//    //
//    boost::shared_ptr<ISFSObject> params (new SFSObject());
//    params->PutUtfString("aI", myself->Name());
//    boost::shared_ptr<IRequest> request (new ExtensionRequest("rgaci", params));
//    GameServer::getSingleton().getSmartFox()->Send(request);
}

// CCBSelectorResolver interface
SEL_MenuHandler LayerInviteFriendsFacebook::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnBuy", LayerInviteFriendsFacebook::onButtonBuy);
    return NULL;
}

void LayerInviteFriendsFacebook::onButtonBuy(CCObject* pSender)
{
    CCLOG("onButtonBuy");
}

// CCBMemberVariableAssigner interface
bool LayerInviteFriendsFacebook::onAssignCCBMemberVariable(CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "nodeListAvatar", CCNode*, nodeListAvatar);
    return true;
}

void LayerInviteFriendsFacebook::onNodeLoaded( CCNode * pNode,  CCNodeLoader * pNodeLoader)
{
    //Init for table
    tblAvatar = CCTableView::create(this, nodeListAvatar->getContentSize());
    tblAvatar->setDirection(kCCScrollViewDirectionHorizontal);
    tblAvatar->setAnchorPoint(ccp(0, 0));
    tblAvatar->setPosition(ccp(0, 0));
    tblAvatar->setDelegate(this);
    tblAvatar->setVerticalFillOrder(kCCTableViewFillTopDown);
    
    nodeListAvatar->addChild(tblAvatar);
    return;
}

// hàm khi click vào 1 hành của table view
void LayerInviteFriendsFacebook::tableCellTouched(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell){
//    CCLOG("Roomid: %d",cell->getObjectID());
}

// Hàm set giá trị width height cho 1 cell table view
CCSize LayerInviteFriendsFacebook::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    return CCSizeMake(120, 140);
}

// Hàm này tạo 1 tableview Row để add vào table view
CCTableViewCell* LayerInviteFriendsFacebook::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCTableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
        //Chon
        CCSprite* ready = CCSprite::createWithSpriteFrameName("assest/ratio_disable.png");
        ready->setPosition(ccp(15, 119));
        ready->setAnchorPoint(ccp(0, 0));
        cell->addChild(ready);
        //lable chon
        CCLabelTTF *labelChon = CCLabelTTF::create("Chọn", "Helvetica", 14);
        labelChon->setPosition(ccp(40, 119));
        labelChon->setAnchorPoint(ccp(0, 0));
        cell->addChild(labelChon);
        //avatar
        CCSprite* avatar = CCSprite::createWithSpriteFrameName("assest/icon_default.png");
        avatar->setPosition(ccp(0, 18));
        avatar->setAnchorPoint(ccp(0, 0));
        avatar->cocos2d::CCNode::setScale(93/avatar->getContentSize().width, 93/avatar->getContentSize().height);
        cell->addChild(avatar);
        //Coin
        CCLabelTTF *coin = CCLabelTTF::create("20 Ecoins", "Helvetica", 14);
        coin->setPosition(ccp(95/2, 0));
        coin->setAnchorPoint(ccp(0.5, 0));
        cell->addChild(coin);
    }
    else
    {
        //;;;;
    }
    return cell;
}

// Hàm gán giá trị số hàng của table view
unsigned int LayerInviteFriendsFacebook::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return 10;
}

void LayerInviteFriendsFacebook::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
//    if(strcmp("gaic", cmd->c_str())==0){
//        //Insert datas to textfield
//        txtName->setText( param->GetUtfString("aN")->c_str() );
//        txtPhoneNumber->setText( param->GetUtfString("aMo")->c_str() );
//        txtEmail->setText( param->GetUtfString("aM")->c_str() );
//        txtFavour->setText( "" );
//        txtStatus->setText( "" );
//        btnSex->setPosition(ccp(*param->GetBool("aS")==true ? 198 : 140, btnSex->getPositionY()));
//    }
}