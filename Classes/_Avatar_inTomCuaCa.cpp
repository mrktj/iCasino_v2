//
//  _Avatar_inGame_.cpp
//  iCasino_v2
//
//  Created by Thanhhv on 5/15/14.
//
//

#include "_Avatar_inTomCuaCa.h"
#include "SceneManager.h"
#include "GameServer.h"
#include "LayerPlayGamePhom.h"
#include "LayerPlayGameTLMN.h"

vector<string> getArrSplit1(string &S, const char &str);
vector<string> getArrSplit1(string &S, const char &str){
	vector<string> arrStr;
	string::iterator t,t2;

	for (t = S.begin(); t < S.end(); ){
		// Lặp từ vị trí bắt đầu
		t2=find(t, S.end(),str); // Tìm vị trí space ' ' đầu tiên
		//kể từ vị trí t
		if(t!=t2)
			arrStr.push_back(string(t,t2));
		else if (t2 != S.end())
		{
			arrStr.push_back("");
		}

		if (t2 == S.end())
		{
			break;
		} 

		t=t2+1; // chuyển sang vị trí sau
	}
	return arrStr;
}

AvatarInTomCuaCa::~AvatarInTomCuaCa(){

}

void AvatarInTomCuaCa::onExit() {
	CCLOG("onExit: clean LayerAvatarInGame");

	arrName.clear();
	arrFlag.clear();
	arrURL.clear();
	arrAI.clear();
	arrMoney.clear();
	arrMoneyDouble.clear();
	if (chuong) {
		chuong->release();
		chuong=NULL;
	}
}

bool AvatarInTomCuaCa::init() {
	if (!CCLayer::init()) return false;

	this->setAnchorPoint(ccp(0, 0));
	this->setPosition(ccp(0, 0));

	this->typeGame = 0;
	this->myName = SceneManager::getSingleton().getMyName();
	this->myAI   = SceneManager::getSingleton().getMyName();
	
	CCLog("this->myName = SceneManager::getSingleton().getMyName() = %s", this->myName.c_str());

	Avatar *me = new Avatar(false);
	Avatar *user1 = new Avatar(false);
	Avatar *user2 = new Avatar(false);
	Avatar *user3 = new Avatar(false);
	Avatar *user4 = new Avatar(false);
    Avatar *user5 = new Avatar(false);

    me->setScale(0.8);
    user1->setScale(0.8);
    user2->setScale(0.8);
    user3->setScale(0.8);
    user4->setScale(0.8);
    user5->setScale(0.8);
    
	me->autorelease();
	user1->autorelease();
	user2->autorelease();
    user3->autorelease();
	user4->autorelease();
    user5->autorelease();

	me->setPosition(ccp(30, 320));
	user1->setPosition(ccp(80,200));
	user2->setPosition(ccp(30,80));
	user3->setPosition(ccp(WIDTH_DESIGN-10-user3->getSizeThis().width,320));
	user4->setPosition(ccp(WIDTH_DESIGN-60-user4->getSizeThis().width,200));
    user5->setPosition(ccp(WIDTH_DESIGN-10-user5->getSizeThis().width,80));
	

	me->setTag(kuser0);
	user1->setTag(kuser1);
	user2->setTag(kuser2);
    user3->setTag(kuser3);
	user4->setTag(kuser4);
    user5->setTag(kuser5);

	this->addChild(me);
	this->addChild(user1);
	this->addChild(user2);
    this->addChild(user3);
	this->addChild(user4);
    this->addChild(user5);

	chuong = UIImageView::create();
    chuong->setScale(0.7);
	chuong->loadTexture("Chuong.png");
	chuong->setTouchEnabled(false);
	chuong->setVisible(false);
	chuong->setAnchorPoint(ccp(0,0));
	chuong->setPosition(ccp(0,0));
	this->addChild(chuong);




	return true;
}

void AvatarInTomCuaCa::setIcon(int pos, char *url){
	if (this->getUserByPos(pos) == NULL) return;
	this->getUserByPos(pos)->setIcon(url);
}

void AvatarInTomCuaCa::setFlag(int pos, bool isShow){
	if (this->getUserByPos(pos) == NULL) return;
	this->getUserByPos(pos)->setFlag(isShow);
}

void AvatarInTomCuaCa::setName(int pos, string name){
	if (this->getUserByPos(pos) == NULL) return;
	this->getUserByPos(pos)->setName(name);
}

void AvatarInTomCuaCa::setMoney(int pos, string money){
	if (this->getUserByPos(pos) == NULL) return;
	this->getUserByPos(pos)->setMoney(money);
}

void AvatarInTomCuaCa::setMoney(int pos, int money) {
	if (this->getUserByPos(pos) == NULL) return;
	this->getUserByPos(pos)->setMoney(money);
}

void AvatarInTomCuaCa::setMoney(int pos, double money) {
	if (this->getUserByPos(pos) == NULL) return;
	this->getUserByPos(pos)->setMoney(money);
}

void AvatarInTomCuaCa::setReady(int pos, bool isReady){
	if (this->getUserByPos(pos) == NULL) return;
	this->getUserByPos(pos)->setReady(isReady);
}

void AvatarInTomCuaCa::setVisibleLayerInvite(int pos, bool isShow) {
	Avatar *user = this->getUserByPos(pos);
	if (user != NULL)
	{
		user->setVisibleLayerInvite(isShow);
	}
}

void AvatarInTomCuaCa::setUnReadyAllUser() {
	setReady(kuser0, false);
	setReady(kuser1, false);
	setReady(kuser2, false);
    setReady(kuser3, false);
	setReady(kuser4, false);
    setReady(kuser5, false);
}

void AvatarInTomCuaCa::resetGame() {
	// Ready status
	setUnReadyAllUser();
}


void AvatarInTomCuaCa::formatAndStore(const char &c1, const char &c2) {
	vector<string> arrUsers = getArrSplit1(this->listUser, c1);

	// release arr
	arrName.clear();
	arrFlag.clear();
	arrURL.clear();
	arrAI.clear();
	arrMoney.clear();
	arrMoneyDouble.clear();

	for (int i = 0; i < arrUsers.size(); i++) {
		vector<string> arr = getArrSplit1(arrUsers[i], c2);
		if (arr.size() < 3)
		{
			continue;
		}
		string id = arr[0];
		string name = arr[1];
		string flag = arr[2];


		
		arrFlag.push_back(flag);

		// url icon
		boost::shared_ptr<User> userInfo = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(name);

		if (userInfo == NULL)
		{
			continue;
		}
		boost::shared_ptr<string> url = userInfo->GetVariable("aal")->GetStringValue();
		arrURL.push_back(url->c_str());

		// Money
		boost::shared_ptr<double> amf = userInfo->GetVariable("amf")->GetDoubleValue();
		arrMoneyDouble.push_back(*amf);
		arrMoney.push_back(((int)(*amf)));

		// account ID
		boost::shared_ptr<string> aI = userInfo->Name();
		if (aI != NULL){
			CCLog("---------aI= %s", aI->c_str());
			arrAI.push_back(aI->c_str());
		}

		// account Name
		boost::shared_ptr<string> aN = userInfo->GetVariable("aN")->GetStringValue();
		if (aN != NULL) {
			CCLog("---------aN= %s", aN->c_str());
			arrName.push_back(aN->c_str());
		}
	}

	updateUsers();
}

Avatar* AvatarInTomCuaCa::getUserByPos(int pos){
	if (pos < 0) return NULL;
	if (this->getChildByTag(pos) == NULL) return NULL;
	return (Avatar*) this->getChildByTag(pos);
}

int AvatarInTomCuaCa::getIndexInArrByName(string name) {
	for (int i = 0; i < arrName.size(); i++) {
		if (arrName[i] == name) return i;
	}
	return -1;
}
int AvatarInTomCuaCa::getIndexInArrByAccountID(string aI) {
	for (int i = 0; i < arrAI.size(); i++) {
		if (arrAI[i] == aI) return i;
	}
	return -1;
}
int AvatarInTomCuaCa::getPosByName(string pName) {
	return getPosByAccountID(pName);
	int pos = getIndexInArrByName(this->myName);

	if (pos == -1)
	{
		isGuess = true;
		return (getIndexInArrByName(pName));
	}
	// tra lai vi tri
	else{
		isGuess = false;
		int countUser = arrName.size();
		for (int i = 0; i < countUser; i++) {
			if (arrName[i] == pName) {
				if (i == pos) {
					return kuser0;
				} else if (i == (pos + 1) % 6) {
					return kuser1;
				} else if (i == (pos + 2) % 6) {
					return kuser2;
				} else if (i == (pos + 3) % 6) {
					return kuser3;
				} else if (i == (pos + 4) % 6) {
					return kuser4;
				}else if (i == (pos + 5) % 6) {
					return kuser5;
				}

				break;
			}
		}
	}

	return -1;
}

string AvatarInTomCuaCa::getNameByPos(int pPos) {
	int pos = getIndexInArrByName(this->myName);

	if (pos == -1) {
		this->isGuess = true;
		if (pPos < arrName.size() && pPos >= 0) return arrName.at(pPos);
	}
	else {
		this->isGuess = false;
		int countUser = arrName.size();
		switch (pPos) {
		case kuser0:
			return this->myName;
			break;
		case kuser1:
			return arrName[(pPos + 1) % 6];
			break;
		case kuser2:
			return arrName[(pPos + 2) % 6];
			break;

		case kuser3:
			return arrName[(pPos + 3) % 6];
			break;
		case kuser4:
			return arrName[(pPos + 4) % 6];
			break;
            case kuser5:
                return arrName[(pPos + 5) % 6];
                break;
		}
	}

	return "";
}
string AvatarInTomCuaCa::getAccountIDByPos(int pPos) {
	int pos = getIndexInArrByAccountID(this->myAI);

	if (pos == -1) {
		this->isGuess = true;
		if (pPos < arrAI.size() && pPos >= 0) return arrAI.at(pPos);
	}
	else {
		this->isGuess = false;
		int countUser = arrAI.size();
		switch (pPos) {
		case kuser0:
			return this->myAI;
			break;

		case kuser1:
			return arrAI[(pPos + 1) % 6];
			break;

		case kuser2:
			return arrAI[(pPos + 2) % 6];
			break;

		case kuser3:
			return arrAI[(pPos + 3) % 6];
			break;

		case kuser4:
			return arrAI[(pPos + 4) % 6];
			break;
                
            case kuser5:
                return arrAI[(pPos + 5) % 6];
                break;
		}
	}

	return "";
}
void AvatarInTomCuaCa::updateUsers() {
	if (arrName.size() != arrFlag.size() || arrName.size() != arrURL.size() || arrURL.size() != arrAI.size()) {
		return;
	}

	// Mỗi khi nhảy tới đây thì cần làm 2 việc sau:
	// 1. Làm "sạch" toàn bộ các Avatar: 
	//   1.1 Avatar của mình: ẩn đi, trạng thái ready=false, BtnReady -> title="Sẵn sàng", -> visible=false
	//                        ai="";
	//	 1.2 Avatar user khác hiện lên các InviteUser, ready=false
	//   1.3 Toàn bộ thông tin về tiền được reset
	// 2. Cập nhật lại thông tin: 
	//   2.0: 
	//	 2.1: nếu của mình thì hiện lên Avatar, BtnReady->visible=true 
	//   2.2: các user khác thì cập nhật & ẩn đi Invite
	// ---

	// 1. Làm "sạch"
	Avatar *avaMe = getUserByPos(kuser0);
	Avatar *avauser1 = getUserByPos(kuser1);
	Avatar *avauser2 = getUserByPos(kuser2);
    Avatar *avauser3 = getUserByPos(kuser3);
	Avatar *avauser4 = getUserByPos(kuser4);
    Avatar *avauser5 = getUserByPos(kuser5);

	avaMe->setVisible(false);
	avaMe->setTouchEnabled(false);
	avaMe->setReady(false);
	avaMe->setName("");
	avaMe->setMoney("");
	avaMe->setAI("");
	Button* btnReady = NULL;
	
	if (btnReady == NULL) return;
	btnReady->setTitleText("Sẵn sàng");
	btnReady->setEnabled(false);
	//
	avauser1->setVisibleLayerInvite(true);
	avauser1->setReady(false);
	avauser1->setName("");
	avauser1->setMoney("");
	avauser1->setAI("");
	//
	avauser2->setVisibleLayerInvite(true);
	avauser2->setReady(false);
	avauser2->setName("");
	avauser2->setMoney("");
	avauser2->setAI("");
//
    avauser3->setVisibleLayerInvite(true);
	avauser3->setReady(false);
	avauser3->setName("");
	avauser3->setMoney("");
	avauser3->setAI("");
	//
	avauser4->setVisibleLayerInvite(true);
	avauser4->setReady(false);
	avauser4->setName("");
	avauser4->setMoney("");
	avauser4->setAI("");
    //
    avauser5->setVisibleLayerInvite(true);
	avauser5->setReady(false);
	avauser5->setName("");
	avauser5->setMoney("");
	avauser5->setAI("");

    
    
	// 2. Update info :D
	int length = arrName.size();
	for (int i = 0; i < length; i++) {
		string name = arrName[i];
		string flag = arrFlag[i];
		string url  = arrURL[i];
		string aI = arrAI[i];
		int money = arrMoney[i];
		double moneyDouble = arrMoneyDouble.at(i);

		int pos = getPosByAccountID(aI);
		CCLog("------pos=%d", pos);
		if (pos < 0)
			break;
		/*if (!isGuess){
			getUserByPos(kuser2)->setVisible(false); 
			getUserByPos(kuser2)->setTouchEnabled(false);
			getUserByPos(kuser2)->setPositionY(-200);
		}
		else {
			getUserByPos(kuser2)->setVisible(true);
			getUserByPos(kuser2)->setTouchEnabled(true);
			getUserByPos(kuser2)->setPositionY(10);
		}*/
		Avatar *user = getUserByPos(pos);
		user->setName(name);
		user->setFlag(atoi(flag.c_str()) == 1);
		bool meIsBoss = (atoi(flag.c_str()) == 1);
		user->setIcon(url);
		user->setAI(aI);
		user->setMoney(money);
		user->setMoney(moneyDouble);
		if(pos == kuser0){
			this->getUserByPos(kuser2)->setMeIsBoss(meIsBoss);
			this->getUserByPos(kuser1)->setMeIsBoss(meIsBoss);
            this->getUserByPos(kuser3)->setMeIsBoss(meIsBoss);
			this->getUserByPos(kuser4)->setMeIsBoss(meIsBoss);
            this->getUserByPos(kuser5)->setMeIsBoss(meIsBoss);

		}
		if (pos == kuser0)
		{
			user->setVisible(true);
			user->setTouchEnabled(true);
			btnReady->setEnabled(true);
		}
		
        user->setVisibleLayerInvite(false);
		
	}
}

void AvatarInTomCuaCa::setPosChuong(int pos){
	bool meIsBoss = false;
	switch (pos) {
	case kuser3:
		chuong->setPosition(ccp(WIDTH_DESIGN-115,410));
		chuong->setVisible(true);
		break;
	case kuser4:
		chuong->setPosition(ccp(WIDTH_DESIGN-170,290));
		chuong->setVisible(true);
		break;
	case kuser1:
		chuong->setPosition(ccp(80,290));
		chuong->setVisible(true);
		break;
	case kuser2:
		chuong->setPosition(ccp(30,170));
		chuong->setVisible(true);
        break;
    case kuser5:
        chuong->setPosition(ccp(WIDTH_DESIGN-115,170));
        chuong->setVisible(true);
        break;
	case kuser0:
		chuong->setPosition(ccp(30,410));
		chuong->setVisible(true);
		meIsBoss = true;
		break;
	default:
		break;
	}
	//

	this->getUserByPos(kuser1)->setMeIsBoss(meIsBoss);
    this->getUserByPos(kuser2)->setMeIsBoss(meIsBoss);
    this->getUserByPos(kuser3)->setMeIsBoss(meIsBoss);
	this->getUserByPos(kuser4)->setMeIsBoss(meIsBoss);
    this->getUserByPos(kuser5)->setMeIsBoss(meIsBoss);
}


void AvatarInTomCuaCa::resetAll(){
	setFlag(kuser0, false);
	setFlag(kuser3, false);
	setFlag(kuser4, false);
	setFlag(kuser1, false);
	setFlag(kuser2, false);
    setFlag(kuser5, false);

	setName(kuser0, "");
	setName(kuser3, "");
	setName(kuser4, "");
	setName(kuser1, "");
	setName(kuser2, "");
    setName(kuser5, "");

	setMoney(kuser0, "");
	setMoney(kuser3, "");
	setMoney(kuser4, "");
	setMoney(kuser1, "");
	setMoney(kuser2, "");
    setMoney(kuser5, "");

	

	this->getUserByPos(kuser3)->setVisibleLayerInvite(true);
	this->getUserByPos(kuser4)->setVisibleLayerInvite(true);
	this->getUserByPos(kuser1)->setVisibleLayerInvite(true);
	this->getUserByPos(kuser2)->setVisibleLayerInvite(true);
    this->getUserByPos(kuser5)->setVisibleLayerInvite(true);
}
void AvatarInTomCuaCa::showChatByPos(int pos, string mes)
{
	Chat *newMes = new Chat(mes, pos);
	newMes->setAnchorPoint(ccp(newMes->getContentSize().width/2,0));
	CCPoint point;

	switch (pos) {
	case kuser0:
		point.setPoint((WIDTH_DESIGN/2-newMes->getSize().width/2), 30);
		
		break;

	case kuser1:
		point.setPoint(50, 320);
		break;

	case kuser2:
		point.setPoint(50, 180);
		break;

	case kuser3:
		point.setPoint(WIDTH_DESIGN - newMes->getSize().width, 380);
		break;

	case kuser4:
		point.setPoint(WIDTH_DESIGN - newMes->getSize().width, 180);
		break;
    	case kuser5:
            point.setPoint(WIDTH_DESIGN - newMes->getSize().width, 180);
            break;
	default:
		point.setPoint((WIDTH_DESIGN - newMes->getSize().width) / 2, (HEIGHT_DESIGN - newMes->getSize().height) / 2);
		newMes->setStatusByServer(true);
		break;
	}

	newMes->setPosition(point);
	this->addChild(newMes);
}
int AvatarInTomCuaCa::getPosByAccountID(string aI) {
	int pos = getIndexInArrByAccountID(this->myAI);
	//CCLog("getPosByAccountID aI=%s, myAI=%s, pos=%d", aI.c_str(), myAI.c_str(), pos);

	if (pos == -1)
	{
		isGuess = true;
		return (getIndexInArrByAccountID(aI));
	}
	// tra lai vi tri
	else{
		isGuess = false;
		int countUser = arrAI.size();
		for (int i = 0; i < countUser; i++) {
			if (arrAI[i] == aI) {
				if (i == pos) {
					return kuser0;
				} else if (i == (pos + 1) % 6) {
					return kuser1;
				} else if (i == (pos + 2) % 6) {
					return kuser2;
				} else if (i == (pos + 3) % 6) {
					return kuser3;
				} else if (i == (pos + 4) % 6) {
					return kuser4;
				} else if (i == (pos + 5) % 6) {
					return kuser5;
				}

				break;
			}
		}
	}

	return -1;
}