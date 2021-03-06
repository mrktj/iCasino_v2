﻿//
//  AvatarLayer.cpp
//  iCasino_v2
//
//  Created by Thanhhv on 5/14/14.   
//
//
//

#include "_Avatar_.h"
#include "SceneManager.h"
#include "mUtils.h"

#include "cocos2d.h"

USING_NS_CC;

mUtils mu;

Avatar::Avatar(bool isMe):
timer(NULL)
,layerWidget(NULL)
,layerInvite(NULL)
,bg(NULL)
,icon(NULL)
,flag(NULL)
,name(NULL)
,money(NULL)
{
	imagedownloader = new ImageDownloader();
	meIsBoss = false;
	// khoi tao
	this->isMe = isMe;
	this->setAnchorPoint(ccp(0, 0));
	layerPlayerInfo = NULL;


	if (isMe) {
		sizeThis.setSize(196, 56);

		CCSprite *time = CCSprite::create("frame_time_2.png");
		timer = CCProgressTimer::create(time);
		timer->setType(kCCProgressTimerTypeRadial);
		timer->setPosition(ccp(getSizeThis().width / 2, getSizeThis().height / 2+1.68));
		this->addChild(timer);

		// widget
		layerWidget = UILayer::create();
		layerWidget->setPosition(timer->getPosition());
		this->addChild(layerWidget);

		// background
		bg = ImageView::create();
		bg->loadTexture("frame_me_info.png");
		bg->setTouchEnabled(true);
		bg->addTouchEventListener(this, toucheventselector(Avatar::onTouch));

		// icon
		this->sizeIcon.setSize(58, 50);
		icon = ImageView::create();
		icon->loadTexture("icon_default.png");
		icon->setPositionX(-(getSizeThis().width / 2 - icon->getContentSize().width / 2));

		// flag
		flag =  ImageView::create();
		flag->loadTexture("flag.png");
		flag->setPosition(ccp(getSizeThis().width / 2, getSizeThis().height / 2));
		flag->setVisible(false);

		// name
		name = CCLabelTTF::create("", "Arial", 12, CCSizeMake(getSizeThis().width / 2, getSizeThis().height / 2.5), kCCTextAlignmentLeft);
		name->setPosition(ccp(20, 10));

		// money
		money = CCLabelTTF::create("", "Arial", 12, CCSizeMake(getSizeThis().width / 2, getSizeThis().height / 2.5), kCCTextAlignmentLeft);
		money->setPosition(ccp(20, -14));
		money->setColor(ccYELLOW);

		layerWidget->addWidget(bg);
		//layerWidget->addWidget(icon);
		layerWidget->addWidget(flag);
		layerWidget->addChild(name);
		layerWidget->addChild(money);
	}
	else {
		sizeThis.setSize(102, 137);

		// frame_time
		CCSprite *frame_time = CCSprite::create("frame_time.png");
		timer = CCProgressTimer::create(frame_time);
		timer->setType(kCCProgressTimerTypeRadial);
		timer->setPosition(ccp(getSizeThis().width / 2, getSizeThis().height / 2));
		this->addChild(timer);

		layerWidget = UILayer::create();
		layerWidget->setAnchorPoint(ccp(0.5, 0.5));
		layerWidget->setPosition(ccp(getSizeThis().width / 2, getSizeThis().height / 2));

		// frame_user
		bg = ImageView::create();
		bg->loadTexture("frame_user.png");
		bg->setTouchEnabled(true);
		bg->addTouchEventListener(this, toucheventselector(Avatar::onTouch));

		// icon
		this->sizeIcon.setSize(90, 85);
		icon = ImageView::create();
		icon->loadTexture("icon_default.png");

		// flag
		flag = ImageView::create();
		flag->loadTexture("flag.png");
		flag->setPosition(ccp(getSizeThis().width / 2, getSizeThis().height / 2));
		flag->setVisible(false);

		// name
		name = CCLabelTTF::create("", "Arial", 12, CCSizeMake(getSizeThis().width - 2, 15), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		name->setPositionY(getSizeThis().height / 2 - 17);

		// money
		money = CCLabelTTF::create("", "Arial", 12, CCSizeMake(getSizeThis().width-14, 15), kCCTextAlignmentCenter);
		money->setPositionY(-getSizeThis().height / 2 + 17);
		money->setColor(ccYELLOW);

		layerWidget->addWidget(bg);
		layerWidget->addWidget(flag);
		layerWidget->addChild(name);
		layerWidget->addChild(money);

		// invite
		layerInvite = UILayer::create();
		layerInvite->setAnchorPoint(ccp(0.5, 0.5));
		layerInvite->setPosition(ccp(getSizeThis().width / 2, getSizeThis().height / 2));

		ImageView *invite = ImageView::create();
		invite->loadTexture("user_invite.png");
		layerInvite->addWidget(invite);

		layerWidget->setVisible(false);
		this->addChild(layerWidget);
		this->addChild(layerInvite);

		//ntf for Chan (dautv)
		lblNTF = UILabelBMFont::create();
		lblNTF->setFntFile("tttt.fnt");
		lblNTF->setText("");
		lblNTF->setPosition(ccp(getSizeThis().width / 2, getSizeThis().height / 2));
		//lblNTF->setScale(1.2);
		lblNTF->setColor(ccGREEN);
		lblNTF->setVisible(false);
		this->addChild(lblNTF);

	}
	nodeIcon = CCNode::create();
	nodeIcon->setContentSize( sizeIcon );
	nodeIcon->setAnchorPoint(ccp(0.5, 0.5));
	if (this->isMe) {
		nodeIcon->setPositionX(-(getSizeThis().width / 2 - sizeIcon.width / 2 - 3));
		nodeIcon->setPositionY(2);
	}
	layerWidget->addChild(nodeIcon);
}

Avatar::~Avatar() {
	if (timer)
	{
		timer->release();
		timer=NULL;
	}
	if (layerWidget)
	{
		layerWidget->release();
		layerWidget=NULL;
	}
	if (layerInvite)
	{
		layerInvite->release();
		layerInvite=NULL;
	}
	if (icon)
	{
		icon->release();
		icon=NULL;
	}
	if (flag)
	{
		flag->release();
		flag=NULL;
	}
	if (name)
	{
		name->release();
		name=NULL;
	}
	if (money)
	{
		money->release();
		money=NULL;
	}
	if (layerPlayerInfo) {
		layerPlayerInfo->release();
		layerPlayerInfo=NULL;
	}
	CC_SAFE_DELETE(imagedownloader);
	/*if (layerMoiChoi) {
		layerMoiChoi->release();
		layerMoiChoi=NULL;
	}*/
}

CCSize Avatar::getSizeThis() {
	return sizeThis;
}

void Avatar::onTouch(CCObject *pObject, TouchEventType pType) {
	if (pType == TOUCH_EVENT_ENDED) {	
		if( this->isMe )
			return;
		int length = this->nameString.length();
		if (length == 0)
		{
			CCLog("Invite User!!!!!!!!!!!!!");
			CCNodeLoaderLibrary* ccNodeLoaderLibrary = SceneManager::getSingleton().getNodeLoaderLibrary();
			ccNodeLoaderLibrary->unregisterCCNodeLoader("LayerSMS");
			ccNodeLoaderLibrary->registerCCNodeLoader("LayerSMS",   LayerMoiChoiLoader::loader());
			CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			if (ccbReader)
			{
				layerMoiChoi = (LayerMoiChoi *)ccbReader->readNodeGraphFromFile( "LayerSMS.ccbi" );
				ccbReader->release();
			}
			layerMoiChoi->loadAllDatas();
			this->getParent()->getParent()->addChild(layerMoiChoi, 10000);
		}
		else{
			CCLog("info of name= %s", this->nameString.c_str());

			CCNodeLoaderLibrary* ccNodeLoaderLibrary = SceneManager::getSingleton().getNodeLoaderLibrary();
			CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			if (ccbReader)
			{
				layerPlayerInfo = (LayerPlayerInfo *)ccbReader->readNodeGraphFromFile( "LayerPlayerInfo.ccbi" );
				ccbReader->release();
			}
			//
			layerPlayerInfo->setFriendId(this->aI);
			layerPlayerInfo->setAvatarUrl(urlAvatar);
			layerPlayerInfo->setIsBossRoom( meIsBoss );
			layerPlayerInfo->reloadAllDatas();
			this->getParent()->getParent()->addChild(layerPlayerInfo, 10000);
		}
	}
}

void Avatar::setVisibleLayerInvite(bool isShow) {
	if (isShow)
	{
		showLayerInvite();
	}
	else hideLayerInvite();
}

void Avatar::hideLayerInvite() {
	if (!this->isMe) {
		layerInvite->setVisible(false);
	}
	layerWidget->setVisible(true);
}

void Avatar::showLayerInvite() {
	if (!this->isMe) {
		layerInvite->setVisible(true);
	}
	layerWidget->setVisible(false);
}

void Avatar::setIcon(string url){
// 	hideLayerInvite();
// 		vector<string> arr = mUtils::splitString(url, '/');
// 		string nameIcon = "icon.png";
// 		if (arr.size() > 0) nameIcon = arr.at(arr.size()-1);
// 	
// 		std::string writablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
// 		writablePath.append(nameIcon);
// 	
// 		CCLog("writablePath.c_str()= %s", writablePath.c_str());
// 	
// 		CCSprite *avatar = CCSprite::create(writablePath.c_str());
// 		if (avatar == NULL) {
// 			CCLog("Load avatar from Server");
// 			downLoadImage(url, nameIcon);
// 		}
// 		else {
// 			CCLog("Load avatar from Device");
// 			setAvatarBySprite(avatar);
// 		}
	imagedownloader->setPointerNodeImage( nodeIcon );
	imagedownloader->downLoadImage(url);
	urlAvatar = url;
}

void Avatar::setFlag(bool isShow){
	if (flag != NULL){
		// hideLayerInvite();
		flag->setVisible(isShow);
	}
}

void Avatar::setName(string name){
	this->nameString = name;
	if (name.c_str() != NULL) {
		// hideLayerInvite();
		this->name->setString(name.c_str());
	}
}

void Avatar::setMoney(string pMoney){
	if (this->money != NULL){
		// hideLayerInvite();
		this->money->setString(pMoney.c_str());
	}
}

void Avatar::setMoney(int money) {
	setMoney(convertMoney(money) + " xu");
}

void Avatar::setMoney(double money) {
	setMoney(convertMoneyFromDouble_Detail(money));
}

void Avatar::setReady(bool isReady) {
	if (this->bg != NULL) {
		// hideLayerInvite();

		if (isReady) {
			if (this->isMe)
				this->bg->loadTexture("frame_me_info_ready.png");
			else this->bg->loadTexture("frame_user_ready.png");
		} else {
			if (this->isMe)
				this->bg->loadTexture("frame_me_info.png");
			else this->bg->loadTexture("frame_user.png");
		}

	}
}

void Avatar::setAI(string ai) {
	this->aI= ai;
}


void Avatar::startTimer() {
	// hideLayerInvite();
	stopTimer();

	CCProgressTo *actionTimer = CCProgressTo::create(60, 100);
	timer->runAction(CCSequence::create(actionTimer,
		CCCallFuncN::create(this, callfuncN_selector(Avatar::callbackTimer)),
		NULL));
}

void Avatar::callbackTimer(CCNode *pSender) {
	stopTimer();
}

void Avatar::stopTimer() {
	if (timer == NULL) {
		return;
	}

	if (timer->numberOfRunningActions() > 0) {
		timer->stopAllActions();
	}
	timer->setPercentage(0);
}

string Avatar::convertMoney(int money){
	//ostringstream oss;
	//oss<<money;
	//return oss.str();

	ostringstream oss;
	/// 
	if (money < 1000) {
		oss.clear();
		oss<<money;
		return (oss.str() + "");
	}
	else if (money >= 1000 && money <= 999999) {
		string hangTram;
		string hangNghin;

		ostringstream oss1;
		// Nghin
		oss1.clear();
		oss1<<(money / 1000);
		hangNghin = oss1.str();

		ostringstream oss2;
		// tram
		int hTram = (money % 1000);
		oss2.clear();
		oss2<<hTram;
		if (hTram < 10) {
			hangTram = "00";
		}
		else if (hTram >= 10 && hTram < 100) {
			hangTram = "0";
		}
		hangTram += oss2.str();

		return (hangNghin + "," + hangTram + "");
	}
	else if (money >= 1000000) {
		string hangTrieu;
		string hangNghin;
		string hangTram;

		ostringstream oss1;
		// Trieu
		oss1.clear();
		oss1<<(money / 1000000);
		hangTrieu = oss1.str();

		// Nghin
		int hNghin = (money - (money / 1000000) * 1000000) / 1000;
		ostringstream oss2;
		oss2.clear();
		oss2<<hNghin;

		if (hNghin < 10)
		{
			hangNghin = "00";
		}
		else if (hNghin >= 10 && hNghin < 100)
		{
			hangNghin = "0";
		}
		hangNghin += oss2.str();

		// Tram
		int hTram = (money % 1000);
		ostringstream oss3;
		oss3.clear();
		oss3<<hTram;

		if (hTram < 10)
		{
			hangTram = "00";
		}
		else if (hTram >= 10 && hTram < 100)
		{
			hangTram = "0";
		}
		hangTram += oss3.str();

		return (hangTrieu + "," + hangNghin + "," + hangTram + "");
	}

	return "";
}

string Avatar::convertMoneyFromDouble(double money) {
	// tiền tỷ
	string mString;
	if (money >= 1000000000) {
		int ti = money / 1000000000;
		mString = convertMoney(ti);
		return (mString + " tỷ");
	} else {
		return (convertMoney((int)money));
	}
		
		
		
		/*if (money >= 1000000) {
		// tiền triệu
		int ti = money / 1000000;
		ostringstream os;
		os<<ti;
		return (os.str() + " triệu");
	} else if (money >= 1000) {
		string hangTram;
		string hangNghin;

		// Nghin
		ostringstream oss1;
		oss1.clear();
		oss1<<(int)(money / 1000);
		hangNghin = oss1.str();

		ostringstream oss2;
		// tram
		int hTram = ((int)money % 1000);
		oss2.clear();
		oss2<<hTram;
		if (hTram < 10) {
			hangTram = "00";
		}
		else if (hTram >= 10 && hTram < 100) {
			hangTram = "0";
		}
		hangTram += oss2.str();

		return (hangNghin + "," + hangTram + " xu");
	} else {
		ostringstream os;
		os<<money;
		return (os.str() + " xu");
	}*/

	return "!";
}

string Avatar::convertMoneyFromDouble_Detail(double money) {
	// nếu tiền nhỏ hơn hàng tỉ: hiển thị => 123,456,789 xu 
	// nếu tiền lên tới hàng tỉ: 
	//		hàng nghìn tỷ: dùng chữ => 2000 tỷ
	//		dưới hàng nghìn tỷ: => 123,456,789k (~123 tỷ...)

	if (money < 1000000000) {
		return convertMoney(money) + " xu";
	}
	else {
		money = money / 1000;

		if (money > 1000000000) {
			return convertMoneyFromDouble(money * 1000);
		}
		else return (convertMoney((int)money) + "k");
	}

	/*if (money < 1000) {
		ostringstream os;
		os<<money;
		return (os.str() + " xu");
	}
	else {
		money = money / 1000;

		if (money > 2000000000) {
			return convertMoneyFromDouble(money * 1000);
		}
		else return (convertMoney((int)money) + "k xu");
	}*/
	

}

// hoangdd
// void Avatar::loadDefaultImage(){
// 	if (icon ==NULL) {
// 		return;
// 	}
// 
// 	CCSprite* pSprite = CCSprite::createWithSpriteFrameName("assest/icon_default.png");
// 	setAvatarBySprite(pSprite);
// }
// 
// void Avatar::downLoadImage(string url, string fileName){
// 	CCLog("url= %s", url.c_str());
// 	if( url.compare("")==0 ){
// 		loadDefaultImage();
// 		return;
// 	}
// 	CCHttpRequest* request = new CCHttpRequest();
// 	request->setUrl(url.c_str());
// 	request->setRequestType(CCHttpRequest::kHttpGet);
// 	request->setResponseCallback(this, httpresponse_selector(Avatar::onImageDownLoaded));
// 	request->setTag(fileName.c_str());
// 	CCHttpClient::getInstance()->send(request);
// 	request->release();
// }
// 
// void Avatar::onImageDownLoaded(CCHttpClient* pSender, CCHttpResponse* pResponse){
// 	CCHttpResponse* response = pResponse;
// 
// 	if (!response)
// 	{
// 		CCLog("No Response");
// 		loadDefaultImage();
// 		return ;
// 	}
// 	int statusCode = response->getResponseCode();
// 
// 	char statusString[64] = {};
// 	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
// 	CCLog("response code: %d", statusCode);
// 
// 	if (!response->isSucceed())
// 	{
// 		CCLog("response failed");
// 		CCLog("error buffer: %s", response->getErrorBuffer());
// 		loadDefaultImage();
// 		return;
// 	}
// 	std::vector<char>*buffer = response->getResponseData();
// 
// 
// 	CCImage * img=new CCImage();
// 	img->initWithImageData(&(buffer->front()), buffer->size());
// 
// 	// Save image file to device.
// 	std::string writablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
// 	writablePath.append(response->getHttpRequest()->getTag());
// 	// add this line
// 	img->saveToFile(writablePath.c_str());
// 
// 	setAvatarByPath(writablePath.c_str());
// }
// 
// void Avatar::setAvatarByPath(string path) {
// 	CCSprite *img = CCSprite::create(path.c_str());
// 	setAvatarBySprite(img);
// }
// 
// void Avatar::setAvatarBySprite(CCSprite* img) {
// 	img->setTag(tagIcon);
// 	CCSize sizeSprite = img->getContentSize();
// 	img->cocos2d::CCNode::setScale(sizeIcon.width / sizeSprite.width, sizeIcon.height / sizeSprite.height);
// 	if (this->isMe) {
// 		img->setPositionX(-(getSizeThis().width / 2 - sizeIcon.width / 2 - 3));
// 		img->setPositionY(2);
// 	}
// 
// 	layerWidget->removeChildByTag(tagIcon);
// 	layerWidget->addChild(img);
// }

void Avatar::setMeIsBoss( bool isBoss )
{
	meIsBoss = isBoss;
}

void Avatar::setLblNTFChan(const char* text){
	lblNTF->setText(text);
	//icon->setColor(ccc3(200,255,255));
}

UILabelBMFont* Avatar::getLblNTFChan(){
	return this->lblNTF;
}

void Avatar::setBlinkAvatar(){
	CCSprite* blinkAvatar = CCSprite::create("frame_time.png");
	blinkAvatar->setPosition(ccp(getSizeThis().width / 2, getSizeThis().height / 2));
	blinkAvatar->setTag(20);
	this->addChild(blinkAvatar);

	CCActionInterval *blink = CCBlink::create(1, 2);
	CCActionInterval *action_back = blink->reverse();
	CCActionInterval* seq = CCSequence::create( blink, action_back, NULL );
	
	blinkAvatar->runAction( CCRepeatForever::create(seq) );
}
