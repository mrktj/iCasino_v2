//
//  LayerChanGame.cpp
//  iCasinov2
//
//  Created by DauA on 6/4/14.
//
//

#include "LayerChanGame.h"
#include "_Background_inGame_.h"
#include "Requests/ExtensionRequest.h"
#include "CardChan.h"
#include "_Chat_.h"
#include "LayerGameChan_XuongU.h"
#include "SliderCustomLoader.h"
#include "SceneManager.h"
#include "LayerGameChan_KetQua.h"
#include "AllData.h"
#define PI 3.141592653589
#define V_REGISTER_LOADER_GLUE(NODE_LIBRARY, CLASS) NODE_LIBRARY->registerCCNodeLoader(#CLASS, CLASS##Loader::loader())

LayerChanGame::LayerChanGame(){
	CCLOG("Nhay vao chan Game");
	EXT_EVENT_REQ_DISCARD = "rqhofc";
	EXT_EVENT_READY_REQ = "rr";
	EXT_EVENT_REQ_DRAW_CARD = "rqdrwc";
	EXT_EVENT_REQ_TAKE_CARD = "rqtkc";
	EXT_EVENT_REQ_DUOI_CARD = "rqduoic";
	EXT_EVENT_REQ_TRENTAY_DETAIL = "rqttdtl";

	EXT_SRVNTF_GAME_MASTER_INFO = "ntfgminfo";
	EXT_SRVNTF_PLAYER_LIST = "ntfpllst";
	EXT_SRVNTF_USER_READY = "ntfurd";
	EXT_EVENT_START = "s";
	EXT_EVENT_END = "e";
	EXT_EVENT_LISTCARD_NTF = "lcntf";
	EXT_SRVNTF_ONHAND_DETAILS = "ntfohdetl";
	EXT_SRVNTF_CURRENT_PLAYING_USER = "ntfcurpu";
	EXT_SRVNTF_NOC_COUNT = "ntfnoccnt";
	EXT_SRVNTF_CARD_ORIGINATION = "ntfcorg";
	EXT_EVENT_RES_DRAW_CARD = "rsdrwc";
	EXT_EVENT_RES_DUOI_CARD = "rsduoic";
	EXT_EVENT_RES_TAKE_CARD = "rstkc";
	EXT_SRVNTF_ANBAO = "ntfanbao";
	EXT_EVENT_RES_DISCARD = "rshofc";
	EXT_EVENT_GAME_RESULT = "grs";
	EXT_SRVNTF_ONE_EXPECTING_U = "ntfoeu";
	EXT_SRVNTF_ONE_EXPECTING_CHIU = "ntfoechiu";	
	EXT_EVENT_RES_U = "rsU";
	EXT_EVENT_REQ_U = "rqU";
	EXT_EVENT_REQ_CHIU_CARD = "rqchiuc";
	EXT_EVENT_REQ_NOC_DETAIL = "rqnocdtl"; 
	EXT_EVENT_RES_CHIU_CARD = "rschiuc";

	_list_user = "";
	mylistCard = "";
	currentPlayer = "";

	countDiscard = 0;

	gameStarted = false;
	flagChiaBai = false;

	createAvatars();
	createCards();
	createButtons();
	

	lblDetail = UILabel::create();
	lblDetail->setText("");
	lblDetail->setFontSize(20);
	lblDetail->setAnchorPoint(ccp(0, 0));
	lblDetail->setPosition(ccp(20, HEIGHT_DESIGN-lblDetail->getContentSize().height-20));
	lblDetail->setColor(ccRED);
	this->addChild(lblDetail);

	GameServer::getSingleton().addListeners(this);
	this->scheduleOnce(schedule_selector(LayerChanGame::sendRequestJoinGame),1.0f);
	SceneManager::getSingleton().hideLoading();
}

LayerChanGame::~LayerChanGame(){
	GameServer::getSingleton().removeListeners(this);
}

void LayerChanGame::onExit()
{
	CCLOG("Deconstructor Game Chan");
	GameServer::getSingleton().removeListeners(this);
}

void LayerChanGame::createAvatars(){
	layerAvatars = LayerAvatarInGame::create();
	layerAvatars->resetAll();
	layerAvatars->getUserByPos(kUserMe)->setVisible(false);
	layerAvatars->getUserByPos(kUserBot)->setVisible(false);
	layerAvatars->getUserByPos(kUserBot)->setPositionY(-200);
	layerAvatars->getUserByPos(kUserBot)->setTouchEnabled(false);
	layerAvatars->getUserByPos(kUserLeft)->setPositionY(layerAvatars->getUserByPos(kUserLeft)->getPositionY() + 30);
	layerAvatars->getUserByPos(kUserRight)->setPositionY(layerAvatars->getUserByPos(kUserRight)->getPositionY() + 30);
	this->addChild(layerAvatars);
}

void LayerChanGame::createCards(){
	layerCardChan = _Layer_CardChan_::create();
	layerCardChan->createAllCards();
	this->addChild(layerCardChan);
}

void LayerChanGame::createButtons(){
	layerButtons = LayerButtonInGame::create();
	this->addChild(layerButtons);

	int w_Button = 129;
	int h_Button = 44;
	int _space = 20;

	Button *btnReady = createButtonWithTitle_Pos("Sẵn Sàng",ccp(WIDTH_DESIGN - w_Button - _space, _space));
	Button *btnTake = createButtonWithTitle_Pos("Đánh",ccp(WIDTH_DESIGN - w_Button - _space, _space));
	Button *btnBoc = createButtonWithTitle_Pos("Bốc", ccp(WIDTH_DESIGN - w_Button - _space, _space));
	Button *btnDuoi = createButtonWithTitle_Pos("Dưới", ccp(WIDTH_DESIGN - w_Button - _space, _space));

	Button *btnEate = createButtonWithTitle_Pos("Ăn", ccp(WIDTH_DESIGN - w_Button - _space, h_Button + 30));

	Button *btnU = createButtonWithTitle_Pos("Ù", ccp(WIDTH_DESIGN - w_Button * 2 - 30, 20));
	Button *btnChiu = createButtonWithTitle_Pos("Chíu", ccp(WIDTH_DESIGN - w_Button * 2 - 30, h_Button + 30));

	btnReady->setEnabled(true);
	btnTake->setEnabled(false);
	btnBoc->setEnabled(false);
	btnDuoi->setEnabled(false);
	btnEate->setEnabled(false);
	btnU->setEnabled(false);
	btnChiu->setEnabled(false);

	btnReady->setTag(cTag_btnReady);
	btnTake->setTag(cTag_btnTake);
	btnBoc->setTag(cTag_btnBoc);
	btnDuoi->setTag(cTag_btnDuoi);
	btnEate->setTag(cTag_btnEate);
	btnU->setTag(cTag_btnU);
	btnChiu->setTag(cTag_btnChiu);

	btnReady->addTouchEventListener(this, toucheventselector(LayerChanGame::btn_ready_click));
	btnTake->addTouchEventListener(this, toucheventselector(LayerChanGame::btn_take_click));
	btnBoc->addTouchEventListener(this, toucheventselector(LayerChanGame::btn_boc_click));
	btnDuoi->addTouchEventListener(this, toucheventselector(LayerChanGame::btn_Duoi_click));
	btnEate->addTouchEventListener(this, toucheventselector(LayerChanGame::btn_eate_click));
	btnU->addTouchEventListener(this, toucheventselector(LayerChanGame::btn_U_Click));
	btnChiu->addTouchEventListener(this, toucheventselector(LayerChanGame::btn_Chiu_Click));

	layerButtons->addWidget(btnReady);
	layerButtons->addWidget(btnTake);
	layerButtons->addWidget(btnBoc);
	layerButtons->addWidget(btnDuoi);
	layerButtons->addWidget(btnEate);
	layerButtons->addWidget(btnU);
	layerButtons->addWidget(btnChiu);

}

void LayerChanGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if(strcmp("hbc", cmd->c_str())==0){
		return;
	}
	CCLOG("cmd = %s",cmd->c_str());

	//player list
	if(strcmp(EXT_SRVNTF_PLAYER_LIST.c_str(), cmd->c_str())==0){
		boost::shared_ptr<string> lu = param->GetUtfString("lu");
		string _lu = "";
		if( lu != NULL){
			_lu = lu->c_str();
			_list_user = lu->c_str();
			updateUser(_list_user);
			CCLOG("List user update: %s",_list_user.c_str());
			CCLOG("EXT_SRVNTF_PLAYER_LIST");
		}
	}

	//user ready
	else if (strcmp(EXT_SRVNTF_USER_READY.c_str(),cmd->c_str())==0){
		boost::shared_ptr<string> user = param->GetUtfString("usrn");
		string _user = "";
		if (user != NULL) {
			_user = user->c_str();
			setUserReady(_user);
		}

		//wwhen user list user
		CCLOG("user ready: %s",_user.c_str());
		CCLOG("EXT_SRVNTF_USER_READY");
	}

	// game start
	else if (strcmp(EXT_EVENT_START.c_str(), cmd->c_str())==0){
		gameStarted = true;
		layerAvatars->setUnReadyAllUser();
		CCLOG("EXT_EVENT_START");
	}

	//list card ntf
	else if(strcmp(EXT_EVENT_LISTCARD_NTF.c_str(), cmd->c_str())==0){
		boost::shared_ptr<string> usrn = param->GetUtfString("usrn");
		boost::shared_ptr<string> lc = param->GetUtfString("lc");

		string _usrn = "";
		string _lc = "";

		if (usrn != NULL) {
			_usrn = usrn->c_str();
		}
		if (lc != NULL){
			_lc = lc->c_str();
		}

		if (!flagChiaBai) {
			if (strcmp(_usrn.c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str())==0) {
				if(gameStarted){
					layerCardChan->setMyListCards(_lc);
				}
				flagChiaBai = true;
			}
		}else{
			if (strcmp(_usrn.c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str())==0) {
				mylistCard = mUtils::splitString(_lc, '/')[0];
				if (mylistCard != "") {
					layerCardChan->sortMyListCards(mylistCard);
				}
			}
		}
		CCLOG("EXT_EVENT_LISTCARD_NTF");
	}

	//noc count
	else if (strcmp(EXT_SRVNTF_NOC_COUNT.c_str(), cmd->c_str())==0){
		boost::shared_ptr<long> noccount = param->GetInt("noccount");
		if (noccount != NULL) {
			CCLOG("noc count: %ld",*noccount);
		}
		CCLOG("EXT_SRVNTF_NOC_COUNT");
	}

	//chi tiet (detail ca la bao tren tay)
	else if (strcmp(EXT_SRVNTF_ONHAND_DETAILS.c_str(), cmd->c_str())==0){
		boost::shared_ptr<string> ohdetl = param->GetUtfString("ohdetl");
		boost::shared_ptr<string> usrn = param->GetUtfString("usrn");

		string _ohdetl = "";
		string _usrn = "";
		if (ohdetl != NULL) {
			_ohdetl = ohdetl->c_str();
		}
		if (usrn != NULL) {
			_usrn = usrn->c_str();
		}
		if (strcmp(_usrn.c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str())==0) {
			CCLOG("detail %s: ",_ohdetl.c_str());
			////Mô tả các lá bài trên tay: Chắn:Ba Đầu:Cạ:Què
			vector<string> detail = mUtils::splitString(_ohdetl, ':');
			lblDetail->setText("Chắn: "+detail[0]+" Ba Đầu: "+detail[1]+" Cạ: "+detail[2]+ " Què: "+detail[3]);
		}
		CCLOG("EXT_SRVNTF_ONHAND_DETAILS");
	}

	//current player
	else if (strcmp(EXT_SRVNTF_CURRENT_PLAYING_USER.c_str(), cmd->c_str())==0){
		boost::shared_ptr<string> usrn = param->GetUtfString("usrn");
		if (usrn != NULL) {
			currentPlayer = usrn->c_str();
			setCurrentPlayer(currentPlayer,countDiscard);
		}
		CCLOG("EXT_SRVNTF_CURRENT_PLAYING_USER");
	}

	//boc noc
	else if (strcmp(EXT_EVENT_RES_DRAW_CARD.c_str(), cmd->c_str())==0){
		boost::shared_ptr<long> rescode = param->GetInt("rscode");
		if(rescode != NULL){
			CCLOG("Resuilt code Draw: %ld",*rescode);
			if (*rescode == 0) {
				getButtonByTag(cTag_btnTake)->setEnabled(false);
				getButtonByTag(cTag_btnBoc)->setEnabled(false);
				getButtonByTag(cTag_btnEate)->setEnabled(true);
				getButtonByTag(cTag_btnDuoi)->setEnabled(true);
			}
		}
		CCLOG("EXT_EVENT_RES_DRAW_CARD");
	}

	//dưới card
	else if(strcmp(EXT_EVENT_RES_DUOI_CARD.c_str(), cmd->c_str())==0){
		boost::shared_ptr<long> rescode = param->GetInt("rscode");
		if (rescode != NULL) {

		}
		CCLOG("EXT_EVENT_RES_DUOI_CARD");
	}

	//TakeCards
	else if (strcmp(EXT_EVENT_RES_TAKE_CARD.c_str(), cmd->c_str())==0){
		boost::shared_ptr<long> rescode = param->GetInt("rscode");
		if (rescode != NULL) {
			whenUserTakeCards(*rescode);
		}
		CCLOG("EXT_EVENT_RES_TAKE_CARD");
	}

	//card origition
	else if (strcmp(EXT_SRVNTF_CARD_ORIGINATION.c_str(), cmd->c_str())==0){
		boost::shared_ptr<string> f_user = param->GetUtfString("funa");
		boost::shared_ptr<string> t_user = param->GetUtfString("tuna");

		boost::shared_ptr<unsigned char> cardnu = param->GetByte("cardnu");
		boost::shared_ptr<unsigned char> cardsu = param->GetByte("cardsu");
		boost::shared_ptr<unsigned char> crdorg = param->GetByte("crdorg");

		string _f_user = "";
		string _t_user = "";
		string _cardnu = "";
		string _cardsu = "";
		int _crdorg = -1;

		if (f_user != NULL) {
			_f_user = f_user->c_str();
		}
		if (t_user != NULL) {
			_t_user = t_user->c_str();
		}
		if (cardnu != NULL) {
			int number = *cardnu;
			_cardnu = boost::to_string(number);
			CCLOG("cardnu: %s",_cardnu.c_str());
		}
		if (cardsu != NULL) {
			int suite = *cardsu;
			_cardsu = boost::to_string(suite);
			CCLOG("cardsu: %s",_cardsu.c_str());
		}
		if (crdorg != NULL) {
			_crdorg = *crdorg;
			CCLOG("crdorg: %d",_crdorg);
		}
		if(_crdorg == 6){
			countDiscard++;
		}

		eventTakeCards(_f_user, _t_user, _cardnu, _cardsu, _crdorg);
		CCLOG("EXT_SRVNTF_CARD_ORIGINATION");
	}

	//An bao
	else if (strcmp(EXT_SRVNTF_ANBAO.c_str(),cmd->c_str())==0){
		boost::shared_ptr<long> anbaores = param->GetInt("anbaores");
		if (anbaores != NULL) {
			error_AnBao(*anbaores);
		}
		CCLOG("EXT_SRVNTF_ANBAO");
	}

	// Chíu
	else if (strcmp(EXT_SRVNTF_ONE_EXPECTING_CHIU.c_str(),cmd->c_str()) == 0)
	{
		boost::shared_ptr<string> usrn = param->GetUtfString("usrn");
		if (usrn != NULL)
		{
			CCLOG("Co nguoi chiu: %s",usrn->c_str());
			whenConguoi_Chiu(usrn->c_str());
		}
		CCLOG("EXT_SRVNTF_ONE_EXPECTING_CHIU");
	}

	//Bao chiu thanh cong
	else if(strcmp(EXT_EVENT_RES_CHIU_CARD.c_str(),cmd->c_str()) == 0)
	{
		boost::shared_ptr<long> rscode = param->GetInt("rscode");
		if (rscode != NULL)
		{
			if (*rscode == 0)
			{
				getButtonByTag(cTag_btnChiu)->setEnabled(false);
				getButtonByTag(cTag_btnTake)->setEnabled(true);
			}
		}
	}

	//Bao U
	else if(strcmp(EXT_EVENT_RES_U.c_str(),cmd->c_str()) == 0)
	{
		boost::shared_ptr<long> rscode = param->GetInt("rscode");
		if (rscode != NULL)
		{
			if (*rscode == 0)
			{
				CCLOG("Co the xuong U");
				XuongU();
				// Bao U
			}

		}

	}

	//Bao co the u
	else if (strcmp(EXT_SRVNTF_ONE_EXPECTING_U.c_str(), cmd->c_str())==0){
		boost::shared_ptr<string> usrn = param->GetUtfString("usrn");
		if (usrn != NULL) {
			whenConguoi_ChoU(usrn->c_str());
			CCLOG("nguoi cho U: %s",usrn->c_str());
		}
		CCLOG("EXT_SRVNTF_ONE_EXPECTING_U");
	}
	//Danh bai
	else if (strcmp(EXT_EVENT_RES_DISCARD.c_str(), cmd->c_str())==0){
		boost::shared_ptr<long> rscode = param->GetInt("rscode");
		if (rscode != NULL) {
			if (*rscode != 0) {
				CCLOG("Không đánh được !");
			}
		}
		CCLOG("EXT_EVENT_RES_DISCARD");
	}

	//Bài trên tay người Ù
	else if (strcmp("ntfttdetl", cmd->c_str()) == 0)
	{
		boost::shared_ptr<string> usrn = param->GetUtfString("usrn");
		boost::shared_ptr<string> lc = param->GetUtfString("lc");

		if (usrn == NULL || lc == NULL)
		{
			return;
		}

		CCLOG("nguoi U: %s", usrn->c_str());
		CCLOG("Bài trên tay người Ù: %s", lc->c_str());
		if (strcmp(usrn->c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) != 0)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sounds/Chan/uroi.mp3");
			waitPlayer_ReqU(usrn->c_str(), lc->c_str());
		}
	}

	//Bài còn trong nọc
	else if(strcmp("ntfnocdetl", cmd->c_str()) == 0){
		boost::shared_ptr<string> nocdetl = param->GetUtfString("nocdetl");
		if (nocdetl != NULL)
		{
			CCLOG("Các lá bài còn trong nọc: %s", nocdetl->c_str());
			LayerGameChan_KetQua *KQ = (LayerGameChan_KetQua*)this->getChildByTag(666);
			if (KQ != NULL)
			{
				KQ->setValueNoc(nocdetl->c_str());
			}
		}
	}

	//Resuilt game
	else if (strcmp(EXT_EVENT_GAME_RESULT.c_str(),cmd->c_str())==0){
		boost::shared_ptr<string> rg = param->GetUtfString("rg");
		if( rg != NULL){
			CCLOG("Resuilt game: %s",rg->c_str());
			layerAvatars->stopAllTimer();
			resuiltGame(rg->c_str());
		}
		CCLOG("EXT_EVENT_GAME_RESULT");
	}

	// game end
	else if (strcmp(EXT_EVENT_END.c_str(), cmd->c_str())==0){
		setEndGame();
		CCLOG("EXT_EVENT_END");
	}


}//end extensions

void LayerChanGame::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){

}

void LayerChanGame::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){

}

void LayerChanGame::OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){

}
void LayerChanGame::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){

}

//Send Request join game
void LayerChanGame::sendRequestJoinGame(float dt){
	boost::shared_ptr<ISFSObject> params (new SFSObject());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	boost::shared_ptr<IRequest> request (new ExtensionRequest("rqjg", params, lastRoom));
	GameServer::getSingleton().getSmartFox()->Send(request);
}

// tìm vị trí người chơi so với mình
int LayerChanGame::getPosUserByName(string uid,string _list_user){
	int vt = -1;
	vector<string> list;
	if(_list_user.c_str() != NULL && _list_user != ""){
		list = mUtils::splitString(_list_user, ';');
	}

	for(int i = 0; i < list.size(); i++){
		string _id = mUtils::splitString(list[i], ':')[1];
		
		if(strcmp(_id.c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0){
			vt = i;
			break;
		}
	}

	for(int k = 0; k < list.size(); k++){
		if(strcmp(list[k].c_str(), "")==0)
			continue;
		string player = list[k];
		vector<string> n = mUtils::splitString(player, ':');
		if(strcmp(n[1].c_str(), uid.c_str()) == 0){
			if(k == vt){
				return kUserMe;
			}
			else if(k == (vt + 1) % 4){
				return kUserRight;
			}
			else if(k == (vt + 2) % 4){
				return kUserTop;
			}
			else if(k == (vt + 3) % 4){
				return kUserLeft;
			}
			break;
		}
	}
	return vt;
}

//Update list user
void LayerChanGame::updateUser(string list){
	//31:dautv:1;27:dautv3:0

	layerAvatars->resetAll();
	vector<string> listUser;
	listUser = mUtils::splitString(list, ';');
	CCLOG("Do dai: %ld",listUser.size());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();

	for(int i = 0; i < listUser.size(); i++){
		if(lastRoom == NULL){
			return;
		}
		if(listUser[i].c_str() == NULL)
			continue;
		string player = listUser[i];
		vector<string> n = mUtils::splitString(player, ':');

		if(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetUserByName(n[1]) == NULL){
			continue;
		}

		int _money = 0;
		string _name = "";
		string _url = "";

		boost::shared_ptr<string> name = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetUserByName(n[1])->GetVariable("aN")->GetStringValue();
		boost::shared_ptr<double> money = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetUserByName(n[1])->GetVariable("amf")->GetDoubleValue();
		boost::shared_ptr<string> url = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetUserByName(n[1])->GetVariable("aal")->GetStringValue();

		if (name != NULL) {
			_name = name->c_str();
		}
		if(money != NULL){
			_money = (int)(*money);
		}
		if(url != NULL){
			_url = url->c_str();
		}

		if(strcmp(n[1].c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str())==0){
			layerAvatars->setName(kUserMe, _name);
			layerAvatars->getUserByPos(kUserMe)->setMoney(_money);
			layerAvatars->getUserByPos(kUserMe)->setAI(n[1]);
		}
		else{
			switch (getPosUserByName(n[1], _list_user)) {
			case kUserLeft:
				setInfoAvatar(kUserLeft, n[1], _name, _money, _url);
				break;
			case kUserRight:
				setInfoAvatar(kUserRight, n[1], _name, _money, _url);
				break;
			case kUserTop:
				setInfoAvatar(kUserTop, n[1], _name, _money, _url);
				break;
			}
		}
	}
}

void LayerChanGame::setInfoAvatar(int pos,string aI,string name, int money, string url){
	layerAvatars->getUserByPos(pos)->setVisibleLayerInvite(false);
	layerAvatars->getUserByPos(pos)->setAI(aI);
	layerAvatars->getUserByPos(pos)->setName(name);
	layerAvatars->getUserByPos(pos)->setMoney(money);
	layerAvatars->getUserByPos(pos)->setIcon(url);
}

void LayerChanGame::eventTakeCards(string f_user, string t_user, string cardnu, string cardsu, int crdorg){
	CCLOG("From user: %s, to user: %s",f_user.c_str(),t_user.c_str());
	int fpos = getPosUserByName(f_user, _list_user);
	int tpos = getPosUserByName(t_user, _list_user);
	CCLOG("From %d to %d", fpos, tpos);
	layerCardChan->takeCards(fpos, tpos, cardnu, cardsu, crdorg);
	if (crdorg == 3 && tpos == kUserMe){
		getButtonByTag(cTag_btnTake)->setEnabled(true);
	}
}

Button* LayerChanGame::createButtonWithTitle_Pos(const char *pName, CCPoint pPoint){
	Button* button = Button::create();
	button->setTouchEnabled(true);
	button->setScale9Enabled(false);
	button->loadTextures("ready.png", "ready_selected.png", "");
	button->setTitleText(pName);
	button->setTitleColor(ccRED);
	button->setTitleFontSize(20);
	button->setTitleFontSize(button->getContentSize().height / 2);
	button->setAnchorPoint(ccp(0, 0));
	button->setPosition(pPoint);

	return button;
}

Button* LayerChanGame::createButton_Chan(const char* pName, CCPoint pPoint, const char* pTexture){
	Button* button = Button::create();
	button->setTouchEnabled(true);
	button->setScale9Enabled(false);
	button->loadTextures(pTexture, "ready_selected.png", "");
	button->setTitleText(pName);
	button->setTitleColor(ccRED);
	button->setTitleFontSize(20);
	button->setTitleFontSize(button->getContentSize().height / 2);
	button->setAnchorPoint(ccp(0, 0));
	button->setPosition(pPoint);

	return button;
}

Button* LayerChanGame::getButtonByTag(int pTag){
	Button* button = (Button*) this->layerButtons->getWidgetByTag(pTag);
	return button;
}

void LayerChanGame::playSounds(string url){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(("sounds/Chan/"+url).c_str());
}

//When user take Cards
void LayerChanGame::whenUserTakeCards(long rscode){
	if(rscode == 0){
		getButtonByTag(cTag_btnEate)->setEnabled(false);
		getButtonByTag(cTag_btnBoc)->setEnabled(false);
		getButtonByTag(cTag_btnDuoi)->setEnabled(false);
	}else{
		CCLOG("Ăn không đúng Chắn, Cạ");
		Chat *toast = new Chat("Ăn không đúng Chắn Cạ", -1);
		this->addChild(toast);
	}
}

// set current player
void LayerChanGame::setCurrentPlayer(string uid,int _count){
	layerAvatars->stopAllTimer();
	if (strcmp(uid.c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0) {
		getButtonByTag(cTag_btnReady)->setEnabled(false);
		getButtonByTag(cTag_btnTake)->setEnabled(true);
		if (_count == 0) {
			getButtonByTag(cTag_btnEate)->setEnabled(false);
			getButtonByTag(cTag_btnBoc)->setEnabled(false);
		}else{
			getButtonByTag(cTag_btnEate)->setEnabled(true);
			getButtonByTag(cTag_btnBoc)->setEnabled(true);
		}
	}
	else{
		getButtonByTag(cTag_btnTake)->setEnabled(false);
		getButtonByTag(cTag_btnEate)->setEnabled(false);
		getButtonByTag(cTag_btnBoc)->setEnabled(false);
		getButtonByTag(cTag_btnDuoi)->setEnabled(false);

		switch (getPosUserByName(uid, _list_user)) {
		case kUserLeft:
			layerAvatars->getUserByPos(kUserLeft)->startTimer();
			break;
		case kUserRight:
			layerAvatars->getUserByPos(kUserRight)->startTimer();
			break;
		case kUserTop:
			layerAvatars->getUserByPos(kUserTop)->startTimer();
			break;
		default:
			break;
		}
	}
}

//set user ready
void LayerChanGame::setUserReady(string uid){
	if(strcmp(uid.c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str())==0){
		getButtonByTag(cTag_btnReady)->setEnabled(false);
	}else{
		switch (getPosUserByName(uid, _list_user)) {
		case kUserLeft:
			layerAvatars->getUserByPos(kUserLeft)->setReady(true);
			break;
		case kUserRight:
			layerAvatars->getUserByPos(kUserRight)->setReady(true);
			break;
		case kUserTop:
			layerAvatars->getUserByPos(kUserTop)->setReady(true);
			break;
		default:
			break;
		}
	}
}

//Display loi an bao
void LayerChanGame::error_AnBao(long rscode){
	int t = (int)rscode;
	string str = "";
	switch(t){
	case ANBAO_REASON_NO_PROBLEM:
		break;
	case ANBAO_REASON_AN_CA_DOI_CHO:
		CCLOG("Lỗi ăn báo: Ăn cạ đổi chờ");
		str = "Lỗi ăn báo: Ăn cạ đổi chờ";
		break;
	case ANBAO_REASON_BO_AN_CUATREN_BUT_AN_CUATRI:
		CCLOG("Lỗi ăn báo: không ăn cửa trên nhưng lại ăn cửa trì");
		str = "Lỗi ăn báo: không ăn cửa trên nhưng lại ăn cửa trì";
		break;
	case ANBAO_REASON_DISCARD_SAMEAS_CUATREN_CUATRI_DUOITAY:
		CCLOG("Lỗi ăn báo: Đánh đi 1 con đã có ở cửa trên, cửa trì hoặc dưới tay");
		str = "Lỗi ăn báo: Đánh đi 1 con đã có ở cửa trên, cửa trì hoặc dưới tay";
		break;
	case ANBAO_REASON_ANCA_DANHCA:
		CCLOG("Lỗi ăn báo: Ăn cạ đánh cạ");
		str = "Lỗi ăn báo: Ăn cạ đánh cạ";
		break;
	case ANBAO_TREOTRANH:
		CCLOG("Lỗi ăn báo: Treo tranh");
		str = "Lỗi ăn báo: Treo tranh";
		break;
	case ANBAO_DANH_1_CA_CHI_DUOC_AN_CHAN:
		CCLOG("Lỗi ăn báo: Đã đánh cạ đi thì chỉ được Ăn chắn");
		str = "Lỗi ăn báo: Đã đánh cạ đi thì chỉ được Ăn chắn";
		break;
	case ANBAO_REASON_BOCHAN_ANCA:
		CCLOG("Lỗi ăn báo: Bỏ Chắn ăn cạ");
		str = "Lỗi ăn báo: Bỏ Chắn ăn cạ";
		break;
	case ANBAO_REASON_DANHBAI_GIONG_CHANCA_DA_AN:
		CCLOG("Lỗi ăn báo: Đánh đi lá bài giống chắn hoặc Cạ đã ăn");
		str = "Lỗi ăn báo: Đánh đi lá bài giống chắn hoặc Cạ đã ăn";
		break;
	case ANBAO_REASON_DOI_U_BACHTHUCHI:
		CCLOG("Lỗi ăn báo: Đợi Ăn Ù Bạch thủ Chi");
		str = "Lỗi ăn báo: Đợi Ăn Ù Bạch thủ Chi";
		break;
	case ANBAO_REASON_BOCHAN_DANHCHAN:
		CCLOG("Lỗi ăn báo: Bỏ Chắn đánh chắn");
		str = "Lỗi ăn báo: Bỏ Chắn đánh chắn";
		break;
	case ANBAO_REASON_BOCHAN_ANCHAN:
		CCLOG("Lỗi ăn báo: Bỏ Chắn Ăn Chắn");
		str = "Lỗi ăn báo: Bỏ Chắn Ăn Chắn";
		break;
	case ANBAO_REASON_BOCA_ANCA:
		CCLOG("Lỗi ăn báo: Bỏ Chắn Ăn Cạ");
		str = "Lỗi ăn báo: Bỏ Chắn Ăn Cạ";
		break;
	case ANBAO_REASON_DANHCA_ANCA:
		CCLOG("Lỗi ăn báo: Đánh Cạ Ăn Cạ");
		str = "Lỗi ăn báo: Đánh Cạ Ăn Cạ";
		break;
	case ANBAO_REASON_XECA_ANCA:
		CCLOG("Lỗi ăn báo: Xé cạ Ăn Cạ");
		str = "Lỗi ăn báo: Xé cạ Ăn Cạ";
		break;
	case ANBAO_REASON_XECHAN_ANCA:
		CCLOG("Lỗi ăn báo: Xé Chắn Ăn Cạ");
		str = "Lỗi ăn báo: Xé Chắn Ăn Cạ";
		break;
	case ANBAO_REASON_DANH_ROILAI_AN:
		CCLOG("Lỗi ăn báo: Đánh đi 1 con sau lại ăn đúng con đó");
		str = "Lỗi ăn báo: Đánh đi 1 con sau lại ăn đúng con đó";
		break;
	case ANBAO_REASON_DANH_DI_DOI_CHAN:
		CCLOG("Lỗi ăn báo: Đánh đi cả đôi Chắn");
		str = "Lỗi ăn báo: Đánh đi cả đôi Chắn";
		break;
	case ANBAO_REASON_AN_ROILAI_DANH:
		CCLOG("Lỗi ăn báo: Ăn một con rồi lại đánh đi con đó");
		str = "Lỗi ăn báo: Ăn một con rồi lại đánh đi con đó";
		break;
	case ANBAO_REASON_ANCA_ROILAI_DANH_QUAN_CUNG_HANG:
		CCLOG("Lỗi ăn báo: Ăn cạ rồi lại đánh 1 con cùng hàng");
		str = "Lỗi ăn báo: Ăn cạ rồi lại đánh 1 con cùng hàng";
		break;
	case ANBAO_REASON_CHIUDUOC_NHUNG_LAI_ANTHUONG:
		CCLOG("Lỗi ăn báo: Chíu được nhưng lại ăn thường");
		str = "Lỗi ăn báo: Chíu được nhưng lại ăn thường";
		break;
	case ANBAO_REASON_AN_CHON_CA:
		CCLOG("Lỗi ăn báo: Ăn cạ Chọn Cạ");
		str = "Lỗi ăn báo: Ăn cạ Chọn Cạ";
		break;
	case ANBAO_REASON_CO_CHAN_CAU_CA:
		CCLOG("Lỗi ăn báo: có Chắn Cấu Cạ");
		str = "ăn báo: có Chắn Cấu Cạ";
		break;
	}

	LayerNotification* layer = SceneManager::getSingleton().getLayerNotification();
	if( !SceneManager::getSingleton().showNotification() ){
		CCLOG("NTF Dialog already open!");
		return;
	}
	layer->setNotificationOptions("Lỗi", (str+ ",\n không thể báo ù ván này !").c_str(), false , "", 1, this );
}

//Khi co nguoi cho U
void LayerChanGame::whenConguoi_ChoU(string uid){
	string myName = "";
	myName = GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str();

	if(strcmp(uid.c_str(),myName.c_str()) == 0){
		// Hiển thị button Ù
		getButtonByTag(cTag_btnU)->setEnabled(true);
		if(strcmp(myName.c_str(),currentPlayer.c_str()) == 0)
		{

		}
		else
		{
			getButtonByTag(cTag_btnEate)->setEnabled(true);
		}

	}else{
		Chat *toast = new Chat(""+uid+" Đang chờ Ù, Đợi nhà này sướng", -1);
		this->addChild(toast);

		hideAllButton();

		//start timer của người chơi chờ Ù
		switch(getPosUserByName(uid,_list_user)){
		case kUserLeft:
			layerAvatars->getUserByPos(kUserLeft)->startTimer();
			break;
		case kUserRight:
			layerAvatars->getUserByPos(kUserRight)->startTimer();
			break;
		case kUserTop:
			layerAvatars->getUserByPos(kUserTop)->startTimer();
			break;
		}
	}
}

//Co nguoi co the chiu
void LayerChanGame::whenConguoi_Chiu(string uid){
	string myName = "";
	myName = GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str();

	if (strcmp(uid.c_str(),myName.c_str()) == 0)
	{
		getButtonByTag(cTag_btnChiu)->setEnabled(true);
	}
	else
	{
		if(strcmp(myName.c_str(),currentPlayer.c_str()) == 0){
			hideAllButton();
		}
	}
}

//Xuong U
void LayerChanGame::XuongU(){
	cocos2d::extension::CCBReader * ccbReader = NULL;
	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	V_REGISTER_LOADER_GLUE(ccNodeLoaderLibrary, SliderCustom);

	// register loaders
	ccNodeLoaderLibrary->registerDefaultCCNodeLoaders();
	ccNodeLoaderLibrary->registerCCNodeLoader("LayerGameChan_XuongU",   LayerGameChan_XuongULoader::loader());
	// read main layer
	ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);

	LayerGameChan_XuongU *popUp;

	if (ccbReader)
	{
		popUp = (LayerGameChan_XuongU *)ccbReader->readNodeGraphFromFile( "LayerGameChan_XuongU.ccbi" );
		popUp->setPosition(ccp(10,10));
		this->addChild(popUp);
		ccbReader->release();
	}
}


void LayerChanGame::resuiltGame(string resuilt)
{
	
}

void LayerChanGame::waitPlayer_ReqU(string uid, string lc){
	CCNodeLoaderLibrary* ccNodeLoaderLibrary = SceneManager::getSingleton().getNodeLoaderLibrary();
	CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	ccNodeLoaderLibrary->registerCCNodeLoader("LayerGameChan_KetQua",   LayerGameChan_KetQuaLoader::loader());
	ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	LayerGameChan_KetQua* mLayer;
	if (ccbReader)
	{
		mLayer = (LayerGameChan_KetQua *)ccbReader->readNodeGraphFromFile( "LayerGameChan_KetQua.ccbi" );
		mLayer->setPlayer_U(uid);
		mLayer->setListCard_WinnerUser(lc);
		mLayer->setTag(666);
		this->addChild(mLayer);
		ccbReader->release();
	}
}

void LayerChanGame::notificationCallBack(bool isOK, int tag){
	CCLOG("callbackNtf****");
}

//set End Game
void LayerChanGame::setEndGame(){
	currentPlayer = "";
	mylistCard = "";

	flagChiaBai = false;

	countDiscard = 0;

	layerCardChan->resetAllCards();

	getButtonByTag(cTag_btnBoc)->setEnabled(false);
	getButtonByTag(cTag_btnDuoi)->setEnabled(false);
	getButtonByTag(cTag_btnEate)->setEnabled(false);
	getButtonByTag(cTag_btnTake)->setEnabled(false);
	getButtonByTag(cTag_btnU)->setEnabled(false);
	getButtonByTag(cTag_btnChiu)->setEnabled(false);
	getButtonByTag(cTag_btnReady)->setEnabled(true);
}

//btn Ready
void LayerChanGame::btn_ready_click(CCObject *sender, TouchEventType type){
	if(type == TOUCH_EVENT_ENDED){
		boost::shared_ptr<ISFSObject> parameter (new SFSObject());
		boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		boost::shared_ptr<IRequest> request (new ExtensionRequest(EXT_EVENT_READY_REQ,parameter,lastRoom));
		GameServer::getSingleton().getSmartFox()->Send(request);
	}
}

//btn Take Cards
void LayerChanGame::btn_take_click(CCObject *sender, TouchEventType type){
	if(type == TOUCH_EVENT_ENDED){
		layerCardChan->doDisCards();
	}
}

//btn Bốc bài
void LayerChanGame::btn_boc_click(CCObject *sender, TouchEventType type){
	if(type == TOUCH_EVENT_ENDED){
		//EXT_EVENT_REQ_DRAW_CARD
		boost::shared_ptr<ISFSObject> params (new SFSObject());
		boost::shared_ptr<Room> lstRooms = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		boost::shared_ptr<IRequest> request (new ExtensionRequest(EXT_EVENT_REQ_DRAW_CARD,params,lstRooms));
		GameServer::getSingleton().getSmartFox()->Send(request);
	}
}

//Btn Dưới bài

void LayerChanGame::btn_Duoi_click(CCObject *sender, TouchEventType type){
	if(type == TOUCH_EVENT_ENDED){
		layerCardChan->doDuoiCard();
	}
}

//btn Ăn bài
void LayerChanGame::btn_eate_click(CCObject *sender, TouchEventType type){
	if(type == TOUCH_EVENT_ENDED){
		layerCardChan->doEateCard();
	}
}

void LayerChanGame::btn_U_Click(CCObject *sender, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED)
	{
		CCLOG("Send U to server !");
		boost::shared_ptr<Room> lstRooms = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		boost::shared_ptr<ISFSObject> params (new SFSObject());
		boost::shared_ptr<IRequest> resObj (new ExtensionRequest(EXT_EVENT_REQ_U,params,lstRooms));
		GameServer::getSingleton().getSmartFox()->Send(resObj);
	}
}

void LayerChanGame::btn_Chiu_Click(CCObject *sender, TouchEventType type){
	if (type = TOUCH_EVENT_ENDED)
	{
		layerCardChan->doChiuCard();
	}
}

void LayerChanGame::hideAllButton(){
	getButtonByTag(cTag_btnReady)->setEnabled(false);
	getButtonByTag(cTag_btnEate)->setEnabled(false);
	getButtonByTag(cTag_btnBoc)->setEnabled(false);
	getButtonByTag(cTag_btnDuoi)->setEnabled(false);
	getButtonByTag(cTag_btnTake)->setEnabled(false);
	getButtonByTag(cTag_btnChiu)->setEnabled(false);
	getButtonByTag(cTag_btnU)->setEnabled(false);
}
void LayerChanGame::resetPositionButton(){

}

void LayerChanGame::callBackFunction_LatBai(CCNode *pSend){
	if (strcmp(currentPlayer.c_str(),GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0)
	{
		getButtonByTag(cTag_btnBoc)->setEnabled(false);
		//getButtonByTag(cTag_btnEate)->setPosition(getButtonByTag(cTag_btnBoc)->getPosition());
		getButtonByTag(cTag_btnEate)->setEnabled(true);
		getButtonByTag(cTag_btnDuoi)->setEnabled(true);
	}
}