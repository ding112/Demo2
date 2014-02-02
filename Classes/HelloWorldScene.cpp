#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"  
#include "GameOverScene.h"

USING_NS_CC;

HelloWorld::HelloWorld():_targets(NULL),_projectiles(NULL){
}

HelloWorld::~HelloWorld(){
	if (_targets)
	{
		_targets->release();
		_targets = NULL;
	}
	if (_projectiles)
	{
		_projectiles->release();
		_projectiles=NULL;
	}
}
CCScene* HelloWorld::scene(){
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init(){
	_targets = new cocos2d::CCArray();
	_projectiles = new cocos2d::CCArray();
	_projectileDestorys = 0;

	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		CCSize size = CCDirector::sharedDirector()->getVisibleSize();

		CCSprite* pSprite = CCSprite::create("Player.png",CCRectMake(0,0,27,40));
		CC_BREAK_IF(!pSprite);
		pSprite->setPosition(ccp(pSprite->getContentSize().width/2,size.height/2));
		this->addChild(pSprite);

		this->schedule(schedule_selector(HelloWorld::gameLog),1.0);
		this->schedule(schedule_selector(HelloWorld::update));

		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav",true);

		this->initWithColor(ccc4(255,255,255,255));
		this->setTouchEnabled(true);
		bRet=true;
	} while (0);
	return bRet;
}

void HelloWorld::gameLog(float fl){
	this->addTarget();
}

void HelloWorld::addTarget(){
	CCSprite* pTarget = CCSprite::create("Target.png",CCRectMake(0,0,27,40));
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();

	int minY = pTarget->getContentSize().height/2;
	int maxY = size.height-minY;

	int rangeY = maxY-minY;
	int actualY = (rand()%rangeY)+minY;

	pTarget->setPosition(ccp(size.width+pTarget->getContentSize().width/2,actualY));
	this->addChild(pTarget);
	pTarget->setTag(1);
	_targets->addObject(pTarget);

	int minDuration = 2.0;
	int maxDuration = 4.0;
	int rangDuration = maxDuration-minDuration;
	int actualDuration = (rand()%rangDuration)+minDuration;

	CCFiniteTimeAction* actionMove = CCMoveTo::create(actualDuration,ccp(0-pTarget->getContentSize().width/2,actualY));
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this,callfuncN_selector(HelloWorld::spriteMoveFinished));

	pTarget->runAction(CCSequence::create(actionMove,actionMoveDone,NULL));

}

void HelloWorld::spriteMoveFinished(CCNode* sender){
	CCSprite* pSprite = (CCSprite*)sender;
	this->removeChild(pSprite);
	if(pSprite->getTag()==1){
		_targets->removeObject(pSprite);
		GameOverScene* gameOverScene = GameOverScene::create();
		CCScene* endScene = CCScene::create();
		gameOverScene->getLabel()->setString("You Lose :(");
		endScene->addChild(gameOverScene);
		CCDirector::sharedDirector()->replaceScene(endScene);
	}else if (pSprite->getTag()==2){
		_projectiles->removeObject(pSprite);
	}
}

void HelloWorld::ccTouchesEnded(CCSet* touches,CCEvent* event){
	CCTouch* touch = (CCTouch*)touches->anyObject();
	CCPoint location = touch->getLocationInView();

	location = CCDirector::sharedDirector()->convertToGL(location);

	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	CCSprite* projectile = CCSprite::create("Projectile.png",CCRectMake(0,0,20,20));
	projectile->setPosition(ccp(20,winSize.height/2));

	int offX = location.x-projectile->getPositionX();
	int offY = location.y-projectile->getPositionY();

	if(offX<=0){
		return;
	}
	this->addChild((projectile));
	projectile->setTag(2);
	_projectiles->addObject(projectile);

	float tan = (float)offY/(float)offX;
	int realX = winSize.width+projectile->getContentSize().width/2;
	int realY = tan*realX+projectile->getPosition().y;
	CCPoint realPoint = ccp(realX,realY);

	int offRealX = realX-projectile->getPositionX();
	int offRealY = realY=projectile->getPositionY();
	float length = sqrtf((offRealX*offRealX)+(offRealY*offRealY));
	float velocity = 480/1;
	float realMoveDuration = length/velocity;

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav"); 

	projectile->runAction(CCSequence::create(CCMoveTo::create(realMoveDuration,realPoint),CCCallFuncN::create(this,callfuncN_selector(HelloWorld::spriteMoveFinished)),NULL));
}

void HelloWorld::update(float dt){
	CCArray* projectilesToDel = new CCArray;
	CCArray* targetsToDel = new CCArray;
	CCObject* it = NULL;
	CCObject* jt = NULL;

	CCARRAY_FOREACH(_projectiles,it){
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		CCRect projectileRect = CCRectMake(
			projectile->getPositionX()-projectile->getContentSize().width/2,
			projectile->getPositionY()-projectile->getContentSize().height/2,
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		CCARRAY_FOREACH(_targets,jt){
			CCSprite* target = dynamic_cast<CCSprite*>(jt);
			CCRect targetRect = CCRectMake(
				target->getPositionX()-target->getContentSize().width/2,
				target->getPositionY()-target->getContentSize().height/2,
				target->getContentSize().width,
				target->getContentSize().height);
			if (projectileRect.intersectsRect(targetRect))
			{
				targetsToDel->addObject(target);
				projectilesToDel->addObject(projectile);
				_projectileDestorys++;
				if (_projectileDestorys==4)
				{
					GameOverScene* gameOverScene =(GameOverScene*) GameOverScene::create();
					gameOverScene->getLabel()->setString("You Win");
					CCScene* pScene = CCScene::create();
					pScene->addChild(gameOverScene);
					CCDirector::sharedDirector()->replaceScene(pScene);
				}
				
			}
		}
	}

	CCARRAY_FOREACH(targetsToDel,jt){
		CCSprite* target = dynamic_cast<CCSprite*>(jt);
		_targets->removeObject(target);
		this->removeChild(target,true);
	}
	CCARRAY_FOREACH(projectilesToDel,it){
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		_projectiles->removeObject(projectile);
		this->removeChild(projectile,true);
	}

	projectilesToDel->release();
	targetsToDel->release();
}

void HelloWorld::menuCloseCallback(CCObject* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}