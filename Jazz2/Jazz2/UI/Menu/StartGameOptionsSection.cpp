﻿#include "StartGameOptionsSection.h"

namespace Jazz2::UI::Menu
{
	StartGameOptionsSection::StartGameOptionsSection()
		:
		_selectedIndex(2),
		_selectedPlayerType(0),
		_selectedDifficulty(1),
		_lastPlayerType(0),
		_lastDifficulty(0),
		_imageTransition(1.0f),
		_animation(0.0f)
	{
		_items[(int)Item::Character].Name = "Character"_s;
		_items[(int)Item::Difficulty].Name = "Difficulty"_s;
		_items[(int)Item::Start].Name = "Start"_s;
	}

	void StartGameOptionsSection::OnShow(IMenuContainer* root)
	{
		MenuSection::OnShow(root);
	}

	void StartGameOptionsSection::OnUpdate(float timeMult)
	{
		constexpr int availableCharacters = 3;

		if (_animation < 1.0f) {
			_animation = std::min(_animation + timeMult * 0.016f, 1.0f);
		}

		if (_imageTransition < 1.0f) {
			_imageTransition += timeMult * 0.1f;

			if (_imageTransition > 1.0f) {
				_imageTransition = 1.0f;
			}
		}

		if (_root->ActionHit(PlayerActions::Fire)) {
			ExecuteSelected();
		} else if (_root->ActionHit(PlayerActions::Left)) {
			if (_selectedIndex == 0) {
				if (_selectedPlayerType > 0) {
					StartImageTransition();
					_selectedPlayerType--;
				} else {
					StartImageTransition();
					_selectedPlayerType = availableCharacters - 1;
				}
				_root->PlaySfx("MenuSelect"_s, 0.5f);
			} else if (_selectedIndex == 1) {
				if (_selectedDifficulty > 0) {
					StartImageTransition();
					_selectedDifficulty--;
					_root->PlaySfx("MenuSelect"_s, 0.5f);
				}
			}
		} else if (_root->ActionHit(PlayerActions::Right)) {
			if (_selectedIndex == 0) {
				if (_selectedPlayerType < availableCharacters - 1) {
					StartImageTransition();
					_selectedPlayerType++;
				} else {
					StartImageTransition();
					_selectedPlayerType = 0;
				}
				_root->PlaySfx("MenuSelect"_s, 0.5f);
			} else if (_selectedIndex == 1) {
				if (_selectedDifficulty < 3 - 1) {
					StartImageTransition();
					_selectedDifficulty++;
					_root->PlaySfx("MenuSelect"_s, 0.4f);
				}
			}
		} else if (_root->ActionHit(PlayerActions::Up)) {
			_root->PlaySfx("MenuSelect"_s, 0.5f);
			_animation = 0.0f;
			if (_selectedIndex > 0) {
				_selectedIndex--;
			} else {
				_selectedIndex = (int)Item::Count - 1;
			}
		} else if (_root->ActionHit(PlayerActions::Down)) {
			_root->PlaySfx("MenuSelect"_s, 0.5f);
			_animation = 0.0f;
			if (_selectedIndex < (int)Item::Count - 1) {
				_selectedIndex++;
			} else {
				_selectedIndex = 0;
			}
		} else if (_root->ActionHit(PlayerActions::Menu)) {
			_root->PlaySfx("MenuSelect"_s, 0.6f);
			_root->LeaveSection();
		}
	}

	void StartGameOptionsSection::OnDraw(Canvas* canvas)
	{
		Vector2i viewSize = canvas->ViewSize;
		Vector2f center = Vector2f(viewSize.X * 0.5f, viewSize.Y * 0.5f * 0.8f);

		StringView selectedDifficultyImage;
		switch (_selectedPlayerType) {
			default:
			case 0: selectedDifficultyImage = "MenuDifficultyJazz"_s; break;
			case 1: selectedDifficultyImage = "MenuDifficultySpaz"_s; break;
			case 2: selectedDifficultyImage = "MenuDifficultyLori"_s; break;
		}

		_root->DrawElement("MenuDim"_s, 0, center.X * 0.36f, center.Y * 1.4f, IMenuContainer::ShadowLayer - 2, Alignment::Center, Colorf::White, 24.0f, 36.0f);

		_root->DrawElement(selectedDifficultyImage, _selectedDifficulty, center.X * 0.36f, center.Y * 1.4f + 3.0f, IMenuContainer::ShadowLayer, Alignment::Center, Colorf(0.0f, 0.0f, 0.0f, 0.2f * _imageTransition), 0.88f, 0.88f);

		if (_imageTransition < 1.0f) {
			StringView lastDifficultyImage;
			switch (_lastPlayerType) {
				default:
				case 0: lastDifficultyImage = "MenuDifficultyJazz"_s; break;
				case 1: lastDifficultyImage = "MenuDifficultySpaz"_s; break;
				case 2: lastDifficultyImage = "MenuDifficultyLori"_s; break;
			}
			_root->DrawElement(lastDifficultyImage, _lastDifficulty, center.X * 0.36f, center.Y * 1.4f, IMenuContainer::MainLayer, Alignment::Center, Colorf(1.0f, 1.0f, 1.0f, 1.0f - _imageTransition), 0.88f, 0.88f);
		}

		_root->DrawElement(selectedDifficultyImage, _selectedDifficulty, center.X * 0.36f, center.Y * 1.4f, IMenuContainer::MainLayer, Alignment::Center, Colorf(1.0f, 1.0f, 1.0f, _imageTransition), 0.88f, 0.88f);

		int charOffset = 0;
		for (int i = 0; i < (int)Item::Count; i++) {
		    if (_selectedIndex == i) {
		        float size = 0.5f + IMenuContainer::EaseOutElastic(_animation) * 0.6f;

		        _root->DrawElement("MenuGlow"_s, 0, center.X, center.Y, IMenuContainer::MainLayer, Alignment::Center, Colorf(1.0f, 1.0f, 1.0f, 0.4f * size), (_items[i].Name.size() + 3) * 0.5f * size, 4.0f * size, true);

		        _root->DrawStringShadow(_items[i].Name, charOffset, center.X, center.Y,
		            Alignment::Center, Font::RandomColor, size, 0.7f, 1.1f, 1.1f, 0.4f, 0.9f);
		    } else {
		        _root->DrawStringShadow(_items[i].Name, charOffset, center.X, center.Y, Alignment::Center,
		            Font::DefaultColor, 0.9f);
		    }

		    if (i == 0) {
		        constexpr const StringView playerTypes[] = { "Jazz"_s, "Spaz"_s, "Lori"_s };
		        constexpr Colorf playerColors[] = {
		            Colorf(0.2f, 0.45f, 0.2f, 0.5f),
		            Colorf(0.45f, 0.27f, 0.22f, 0.5f),
		            Colorf(0.5f, 0.45f, 0.22f, 0.5f)
		        };

				constexpr int availableCharacters = 3;

		        float offset, spacing;
		        if (availableCharacters == 1) {
		            offset = 0.0f;
		            spacing = 0.0f;
		        } else if (availableCharacters == 2) {
		            offset = 50.0f;
		            spacing = 100.0f;
		        } else {
		            offset = 100.0f;
		            spacing = 300.0f / availableCharacters;
		        }

		        for (int j = 0; j < availableCharacters; j++) {
		            float x = center.X - offset + j * spacing;
		            if (_selectedPlayerType == j) {
		                _root->DrawElement("MenuGlow"_s, 0, x, center.Y + 28.0f, IMenuContainer::MainLayer, Alignment::Center, Colorf(1.0f, 1.0f, 1.0f, 0.2f), (playerTypes[j].size() + 3) * 0.4f, 2.2f, true);

		                _root->DrawStringShadow(playerTypes[j], charOffset, x, center.Y + 28.0f, Alignment::Center,
		                   playerColors[j], 1.0f, 0.4f, 0.55f, 0.55f, 0.8f, 0.9f);
		            } else {
		                _root->DrawStringShadow(playerTypes[j], charOffset, x, center.Y + 28.0f, Alignment::Center,
		                    Font::DefaultColor, 0.8f, 0.0f, 4.0f, 4.0f, 0.4f, 0.9f);
		            }
		        }

		        _root->DrawStringShadow("<", charOffset, center.X - (100.0f + 40.0f), center.Y + 28.0f, Alignment::Center,
					Font::DefaultColor, 0.7f);
		        _root->DrawStringShadow(">", charOffset, center.X + (100.0f + 40.0f), center.Y + 28.0f, Alignment::Center,
					Font::DefaultColor, 0.7f);

				_items[i].TouchY = center.Y + 28.0f;
		    } else if (i == 1) {
				constexpr const StringView difficultyTypes[] = { "Easy"_s, "Medium"_s, "Hard"_s };

		        for (int j = 0; j < _countof(difficultyTypes); j++) {
		            if (_selectedDifficulty == j) {
		                _root->DrawElement("MenuGlow"_s, 0, center.X + (j - 1) * 100.0f, center.Y + 28.0f, IMenuContainer::MainLayer, Alignment::Center, Colorf(1.0f, 1.0f, 1.0f, 0.2f), (difficultyTypes[j].size() + 3) * 0.4f, 2.2f, true);

		                _root->DrawStringShadow(difficultyTypes[j], charOffset, center.X + (j - 1) * 100.0f, center.Y + 28.0f, Alignment::Center,
						   Colorf(0.501f, 0.501f, 0.501f, 0.5f), 1.0f, 0.4f, 0.55f, 0.55f, 0.8f, 0.9f);
		            } else {
		                _root->DrawStringShadow(difficultyTypes[j], charOffset, center.X + (j - 1) * 100.0f, center.Y + 28.0f, Alignment::Center,
		                    Font::DefaultColor, 0.8f, 0.0f, 4.0f, 4.0f, 0.9f);
		            }
		        }

		        _root->DrawStringShadow("<"_s, charOffset, center.X - (100.0f + 40.0f), center.Y + 28.0f, Alignment::Center,
		            Font::DefaultColor, 0.7f);
		        _root->DrawStringShadow(">"_s, charOffset, center.X + (100.0f + 40.0f), center.Y + 28.0f, Alignment::Center,
		            Font::DefaultColor, 0.7f);

				_items[i].TouchY = center.Y + 28.0f;
			} else {
				_items[i].TouchY = center.Y;
			}

		    center.Y += 70.0f;
		}
	}

	void StartGameOptionsSection::OnTouchEvent(const nCine::TouchEvent& event, const Vector2i& viewSize)
	{
		if (event.type == TouchEventType::Down) {
			int pointerIndex = event.findPointerIndex(event.actionIndex);
			if (pointerIndex != -1) {
				float x = event.pointers[pointerIndex].x * (float)viewSize.X;
				float y = event.pointers[pointerIndex].y * (float)viewSize.Y;
				float halfWidth = viewSize.X * 0.5f;

				if (y < 80.0f) {
					_root->PlaySfx("MenuSelect", 0.5f);
					_root->LeaveSection();
					return;
				}

				for (int i = 0; i < (int)Item::Count; i++) {
					if (std::abs(x - halfWidth) < 150.0f && std::abs(y - _items[i].TouchY) < 30.0f) {
						switch (i) {
							case 0: {
								int selectedSubitem = (x < halfWidth - 50.0f ? 0 : (x > halfWidth + 50.0f ? 2 : 1));
								if (_selectedPlayerType != selectedSubitem) {
									StartImageTransition();
									_selectedPlayerType = selectedSubitem;
									_root->PlaySfx("MenuSelect"_s, 0.5f);
								}
								break;
							}
							case 1: {
								int selectedSubitem = (x < halfWidth - 50.0f ? 0 : (x > halfWidth + 50.0f ? 2 : 1));
								if (_selectedDifficulty != selectedSubitem) {
									StartImageTransition();
									_selectedDifficulty = selectedSubitem;
									_root->PlaySfx("MenuSelect"_s, 0.5f);
								}
								break;
							}
							default: {
								if (_selectedIndex == i) {
									ExecuteSelected();
								} else {
									_root->PlaySfx("MenuSelect"_s, 0.5f);
									_animation = 0.0f;
									_selectedIndex = i;
								}
								break;
							}
						}
						break;
					}
				}
			}
		}
	}

	void StartGameOptionsSection::ExecuteSelected()
	{
		if (_selectedIndex != 2) {
			return;
		}

		// TODO
		/*ControlScheme.IsSuspended = true;

		_root->PlaySound("MenuSelect", 0.6f);
		_root->BeginFadeOut(() = > {
			ControlScheme.IsSuspended = false;

			bool enableReduxMode = Preferences.Get<bool>("ReduxMode", true);

			LevelInitialization levelInit = new LevelInitialization(
				episodeName,
				levelName,
				(GameDifficulty.Easy + selectedDifficulty),
				enableReduxMode,
				false,
				(PlayerType.Jazz + selectedPlayerType)
			);

			if (!string.IsNullOrEmpty(previousEpisodeName)) {
				ref PlayerCarryOver player = ref levelInit.PlayerCarryOvers[0];

				byte lives = Preferences.Get<byte>("EpisodeEnd_Lives_" + previousEpisodeName);
				uint score = Preferences.Get<uint>("EpisodeEnd_Score_" + previousEpisodeName);
				short[] ammo = Preferences.Get<short[]>("EpisodeEnd_Ammo_" + previousEpisodeName);
				byte[] upgrades = Preferences.Get<byte[]>("EpisodeEnd_Upgrades_" + previousEpisodeName);
				bool cheatsUsed = Preferences.Get<bool>("EpisodeEnd_Cheats_" + previousEpisodeName, false);

				if (lives > 0) {
					player.Lives = lives;
				}
				if (score > 0) {
					player.Score = score;
				}
				if (ammo != null) {
					player.Ammo = ammo;
				}
				if (upgrades != null) {
					player.WeaponUpgrades = upgrades;
				}

				levelInit.CheatsUsed = cheatsUsed;
			}

			_root->SwitchToLevel(levelInit);
		});*/

		PlayerType players[] = { (PlayerType)((int)PlayerType::Jazz + _selectedPlayerType) };
		LevelInitialization levelInit("unknown"_s, "share1"_s, (GameDifficulty)((int)GameDifficulty::Easy + _selectedDifficulty),
			true, false, players, _countof(players));
		_root->ChangeLevel(std::move(levelInit));
	}

	void StartGameOptionsSection::StartImageTransition()
	{
		_lastPlayerType = _selectedPlayerType;
		_lastDifficulty = _selectedDifficulty;
		_imageTransition = 0.0f;
	}
}