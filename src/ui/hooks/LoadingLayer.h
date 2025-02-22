#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "../nodes/MinecraftButton.h"
#include "../nodes/MinecraftLabel.h"

class $modify(MyLoadingLayer, LoadingLayer){

	float lastWidth = 1;
	float loadingScaleX = 1.5;
	float loadingScaleY = 1.4;
	bool isInitialOpen = true;

	TodoReturn loadingFinished(){
		LoadingLayer::loadingFinished();

		if(GlobalVars::getSharedInstance()->isInitialLaunch){
			this->scheduleOnce(schedule_selector(MyLoadingLayer::doFadeOut), 0.05f);
		}
	}

	void doFadeOut(float dt){
		CCLayerColor* loadingBar = dynamic_cast<CCLayerColor*>(this->getChildByID("loading-bar"_spr));
		CCLayerColor* loadingBarBG2 = dynamic_cast<CCLayerColor*>(this->getChildByID("loading-bar-bg2"_spr));
		MinecraftLabel* loadingText = dynamic_cast<MinecraftLabel*>(this->getChildByID("loading-text"_spr));

		CCFadeOut* fadeOut = CCFadeOut::create(1.0f);
		loadingBar->runAction(fadeOut);
		CCFadeOut* fadeOut2 = CCFadeOut::create(1.0f);
		loadingBarBG2->runAction(fadeOut2);
		CCFadeOut* fadeOut3 = CCFadeOut::create(1.0f);
		loadingText->runAction(fadeOut3);

		this->scheduleOnce(schedule_selector(MyLoadingLayer::doChangeScene), 1.0f);
	}

	void doChangeScene(float dt){

		CCScene* scene = MenuLayer::scene(false);
		CCDirector::sharedDirector()->pushScene(scene);
		GlobalVars::getSharedInstance()->isInitialLaunch = false;
	}

	bool init(bool p0){
		if(!LoadingLayer::init(p0)){
			return false;
		}

#ifdef GEODE_WINDOWS
		auto hwnd = FindWindowW(NULL, L"Geometry Dash");
    	SetWindowTextA(hwnd, "Minecraft* 2.2");

		std::string path = Mod::get()->getResourcesDir().append("logo.ico").string();


		log::info("{}", path);

		HICON hIcon = (HICON)LoadImage(NULL, path.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

#endif

		CCLabelBMFont* loadingTextOrig = dynamic_cast<CCLabelBMFont*>(this->getChildByID("geode-small-label"));
		loadingTextOrig->setVisible(false);

		this->getChildByID("bg-texture")->setVisible(false);
		this->getChildByID("loading-text")->setVisible(false);
		this->getChildByID("text-area")->setVisible(false);
		this->getChildByID("robtop-logo")->setVisible(false);
		this->getChildByID("gd-logo")->setVisible(false);
		this->getChildByID("fmod-logo")->setVisible(false);
		this->getChildByID("cocos2d-logo")->setVisible(false);
		this->getChildByID("progress-slider")->setVisible(false);
		this->m_sliderBar->setVisible(false);

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		CCLayerColor* layerColor = CCLayerColor::create(ccColor4B{239,50,61,255});
		layerColor->setContentSize(winSize);

		this->addChild(layerColor);

		CCLayerColor* loadingBarBG2 = CCLayerColor::create(ccColor4B{255,255,255,255});
		loadingBarBG2->setContentSize({this->m_fields->loadingScaleX*200+4, this->m_fields->loadingScaleY*5+4});
		loadingBarBG2->ignoreAnchorPointForPosition(false);
		loadingBarBG2->setAnchorPoint({0, 0.5});
		loadingBarBG2->setID("loading-bar-bg2"_spr);
		loadingBarBG2->setPosition({winSize.width/2-100*this->m_fields->loadingScaleX-2, 60});
		this->addChild(loadingBarBG2);

		CCLayerColor* loadingBarBG1 = CCLayerColor::create(ccColor4B{239,50,61,255});
		loadingBarBG1->setContentSize({this->m_fields->loadingScaleX*200+2, this->m_fields->loadingScaleY*5+2});
		loadingBarBG1->ignoreAnchorPointForPosition(false);
		loadingBarBG1->setAnchorPoint({0, 0.5});
		loadingBarBG1->setID("loading-bar-bg1"_spr);

		loadingBarBG1->setPosition({winSize.width/2-100*this->m_fields->loadingScaleX-1.0f, 60});
		this->addChild(loadingBarBG1);

		CCLayerColor* loadingBar = CCLayerColor::create(ccColor4B{255,255,255,255});
		loadingBar->setContentSize({1, 5});
		loadingBar->ignoreAnchorPointForPosition(false);
		loadingBar->setAnchorPoint({0, 0.5});
		loadingBar->setScaleX(this->m_fields->loadingScaleX);
		loadingBar->setScaleY(this->m_fields->loadingScaleY);

		loadingBar->setPosition({winSize.width/2-100*this->m_fields->loadingScaleX, 60});
		loadingBar->setID("loading-bar"_spr);
		this->addChild(loadingBar);

		

		CCSprite* titleSprite = Utils::createSprite("/geode/unzipped/zalphalaneous.minecraft/resources/zalphalaneous.minecraft/mojangstudios.png");

		titleSprite->setPosition({winSize.width/2, winSize.height/2});
		titleSprite->setScale(titleSprite->getScale()*0.3f);

		this->addChild(titleSprite);



		MinecraftLabel* loadingText = MinecraftLabel::create("", "/geode/unzipped/zalphalaneous.minecraft/resources/zalphalaneous.minecraft/minecraft.fnt");
		loadingText->setScale(0.4f);
		loadingText->setPosition({winSize.width/2, 30});

		this->addChild(loadingText);
		loadingText->setID("loading-text"_spr);

		CCScheduler::get()->scheduleSelector(schedule_selector(MyLoadingLayer::myUpdate), this, 0.0, false);

		return true;
	}

	void myUpdate(float dt){

		CCLabelBMFont* loadingTextOrig = dynamic_cast<CCLabelBMFont*>(this->getChildByID("geode-small-label"));

		float currentWidth = this->m_sliderBar->getContentSize().width;

		if(currentWidth > 200){
			currentWidth = 200;
		}

		if(currentWidth != this->m_fields->lastWidth ){
			CCLayerColor* loadingBar = dynamic_cast<CCLayerColor*>(this->getChildByID("loading-bar"_spr));
	
			CCScaleTo* scaleTo = CCScaleTo::create(0.1, currentWidth*this->m_fields->loadingScaleX, this->m_fields->loadingScaleY);
			loadingBar->runAction(scaleTo);

			this->m_fields->lastWidth = currentWidth;
		}


		MinecraftLabel* loadingText = dynamic_cast<MinecraftLabel*>(this->getChildByID("loading-text"_spr));
		loadingText->setString(loadingTextOrig->getString());
	}

};
