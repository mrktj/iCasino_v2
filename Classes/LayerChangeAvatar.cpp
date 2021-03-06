//
//  LayerChangeAvatar.cpp
//  iCasinov2
//
//  Created by AlexDao on 6/6/14.
//
//

#include "LayerChangeAvatar.h"
#include "LayerChangeAvatar.h"
#include "Requests/ExtensionRequest.h"
#include "mUtils.h"
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#include "../CrossPlatform/Android/Android.h"
#endif


using namespace cocos2d;
//using namespace CocosDenshion;

#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#include <jni.h>
extern "C"
{
	void Java_com_game_simple_Game3_setlinkAvata(JNIEnv *env,jobject thiz,jstring link)
	{

		const char* str;
		str = env->GetStringUTFChars(link, false);
		std::string tempStr(str);
		
		if(tempStr.compare(" ")!=0)
		{
			boost::shared_ptr<ISFSObject> params (new SFSObject());
			params->PutUtfString("aal", tempStr.c_str());
			boost::shared_ptr<IRequest> request (new ExtensionRequest("ruali",params));
			GameServer::getSingleton().getSmartFox()->Send(request);	
		}
		else
			CCLog("Upload fail!");
	}
} 
#endif

LayerChangeAvatar::LayerChangeAvatar()
{
	nodeListAvatar = NULL;
	tblAvatar = NULL;
    //
    GameServer::getSingleton().addListeners(this);
	getToken();
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	startTimer();
#else
	CCLog("Ko ho tro nen tang nay !");
#endif
}

LayerChangeAvatar::~LayerChangeAvatar()
{
    GameServer::getSingleton().removeListeners(this);
    //
    CC_SAFE_RELEASE(nodeListAvatar);
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	stopTimer();
#else
	CCLog("Ko ho tro nen tang nay !");
#endif
}

void LayerChangeAvatar::loadAllMyDatas(){
    tblAvatar->reloadData();
}

// CCBSelectorResolver interface
SEL_MenuHandler LayerChangeAvatar::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnUpdate", LayerChangeAvatar::onButtonUpdate);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnDefault", LayerChangeAvatar::onButtonDefault);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnFacebook", LayerChangeAvatar::onButtonFacebook);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnChooseFile", LayerChangeAvatar::onButtonChooseFile);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnCapture", LayerChangeAvatar::onButtonCapture);
    return NULL;
}

void LayerChangeAvatar::onButtonUpdate(CCObject* pSender){
    CCLOG("onButtonUpdate");
	
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
{
		uploadAvatar(token->c_str());
		getToken();
		
}
#endif

}
void LayerChangeAvatar::onButtonDefault(CCObject* pSender){
    CCLOG("onButtonDefault");
    
}
void LayerChangeAvatar::onButtonFacebook(CCObject* pSender){

    
}
void LayerChangeAvatar::onButtonChooseFile(CCObject* pSender){
    CCLOG("onButtonChooseFile");
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	openImage();
#else
	CCLog("Khong ho tro nen tang nay!");
#endif
}
void LayerChangeAvatar::onButtonCapture(CCObject* pSender){
    CCLOG("onButtonCapture");
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	camera();
#else
	CCLog("Khong ho tro nen tang nay!");
#endif
}

// CCBMemberVariableAssigner interface
bool LayerChangeAvatar::onAssignCCBMemberVariable(CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCLOG("LayerChangeAvatar::onAssignCCBMemberVariable, %s", pMemberVariableName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "nodeListAvatar", CCNode*, nodeListAvatar);
    return true;
}

void LayerChangeAvatar::onNodeLoaded( CCNode * pNode,  CCNodeLoader * pNodeLoader)
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
void LayerChangeAvatar::tableCellTouched(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell){
    //    CCLOG("Roomid: %d",cell->getObjectID());
}

// Hàm set giá trị width height cho 1 cell table view
CCSize LayerChangeAvatar::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    return CCSizeMake(120, 123);
	//return CCSizeMake(100, 100);
}

// Hàm này tạo 1 tableview Row để add vào table view
CCTableViewCell* LayerChangeAvatar::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCTableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
        //Chon
        CCSprite* ready = CCSprite::createWithSpriteFrameName("assest/ratio_disable.png");
        ready->setPosition(ccp(15, 100));
        ready->setAnchorPoint(ccp(0, 0));
        cell->addChild(ready);
        //lable chon
        CCLabelTTF *labelChon = CCLabelTTF::create("Chọn", "Helvetica", 14);
        labelChon->setPosition(ccp(40, 100));
        labelChon->setAnchorPoint(ccp(0, 0));
        cell->addChild(labelChon);
        //avatar
        CCSprite* avatar = CCSprite::createWithSpriteFrameName("assest/icon_default.png");
        avatar->setPosition(ccp(0, 0));
        avatar->setAnchorPoint(ccp(0, 0));
        avatar->cocos2d::CCNode::setScale(93/avatar->getContentSize().width, 93/avatar->getContentSize().height);
        cell->addChild(avatar);    }
    else
    {
        //;;;;
    }
    return cell;
}

// Hàm gán giá trị số hàng của table view
unsigned int LayerChangeAvatar::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return 10;
}

void LayerChangeAvatar::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if(strcmp("ire", cmd->c_str())==0)
		{
				token = param->GetUtfString("ire");
				
				CCLog("--%s",token->c_str());
			}
//   if(strcmp("gaic", cmd->c_str())==0){
//        //Insert datas to textfield
//        txtName->setText( param->GetUtfString("aN")->c_str() );
//        txtPhoneNumber->setText( param->GetUtfString("aMo")->c_str() );
//        txtEmail->setText( param->GetUtfString("aM")->c_str() );
//        txtFavour->setText( "" );
//        txtStatus->setText( "" );
//        btnSex->setPosition(ccp(*param->GetBool("aS")==true ? 198 : 140, btnSex->getPositionY()));
//    }
}
void LayerChangeAvatar::getToken()
{
	boost::shared_ptr<ISFSObject> params (new SFSObject());
	params->PutUtfString("aI", GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str());
	//boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<IRequest> request (new ExtensionRequest("ire",params));
	GameServer::getSingleton().getSmartFox()->Send(request);
	CCLog("---%s",GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str());
}