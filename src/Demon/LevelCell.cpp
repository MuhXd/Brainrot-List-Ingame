#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include "../CustomSearch/BrType.hpp"
#include "../url.hpp"
#include "../CustomSearch/Packlist.hpp"
using namespace geode::prelude;
/*
#include <Geode/modify/SetIDPopup.hpp>
class $modify(SetIDPopup) {
	bool init(int current, int begin, int end, gd::string title, gd::string button, bool p5, int p6, float y, bool p8, bool p9) {
		log::debug("{} {} {} {} {} {} {} {} {} {}",current,begin,end,title,button,p5,p6,y,p8,p9);
		return SetIDPopup::init(current,begin,end,title,button,p5,p6,y,p8,p9);
	};

};
*/

class $modify(LevelCell) {
	static void onModify(auto& self) {
        (void)self.setHookPriority("LevelCell::loadFromLevel", INT_MIN/2-1); 
    }
	void loadFromLevel(GJGameLevel* p0) {
		 if (!BrType::LoadedAllLevels) {
            BrType::parseRequestString(level_map);
        }
		LevelCell::loadFromLevel(p0);
		CCNode* difficultyNode;
		if (Loader::get()->isModLoaded("geode.node-ids")) {
			difficultyNode = m_mainLayer->getChildByID("difficulty-container");
			if (!difficultyNode) {
				return;
			};
		}
		CCNode* difficultySpr;
		if (!Loader::get()->isModLoaded("geode.node-ids")) {
			difficultyNode = nullptr;
			for (auto item :  CCArrayExt<CCNode*>(m_mainLayer->getChildren())) {
				if (GJDifficultySprite* spr = item->getChildByType<GJDifficultySprite>(0)) {
					difficultySpr = spr;
					difficultyNode = difficultySpr->getParent(); // great way without node ids
					break;
				}
			};
		} else {difficultySpr = difficultyNode->getChildByID("difficulty-sprite");}
		if (!difficultySpr) {
			return;
		}

		cocos2d::CCPoint difficultyPos = difficultySpr->getPosition() + CCPoint { .0f, .0f };
		if (difficultyNode) {
			auto demonpos = BrType::find(p0->m_levelID);
			if (!demonpos) {
				demonpos = BRPacks::find(p0->m_levelID);
			}
			if (demonpos) {
				if (CCNode* Boo = this->getChildByIDRecursive("uproxide.more_difficulties/more-difficulties-spr")) {
					Boo->setScale(0);
					Boo->setVisible(false);
					typeinfo_cast<CCSprite*>(Boo)->setOpacity(0);
				}
                CCSprite* mdSpr = (!Mod::get()->getSettingValue<bool>("better-face")) ? CCSprite::create("normal_face_with_demon_text.png"_spr) : CCSprite::create("Betterface.png"_spr);
				mdSpr->setPosition(difficultyPos);
				if (mdSpr->getParent() != difficultyNode) difficultyNode->addChild(mdSpr);
				mdSpr->setZOrder(difficultySpr->getZOrder());
				mdSpr->setScale((!Mod::get()->getSettingValue<bool>("better-face")) ? 0.2 : 0.4);
				mdSpr->setAnchorPoint({0.5,0.350});
				typeinfo_cast<CCSprite*>(difficultySpr)->setOpacity(0);
				mdSpr->setID("BRL"_spr);
			}

		} 
	}
}; 