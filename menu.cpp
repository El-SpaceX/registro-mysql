#include "menu.hpp"


void Menu::UpdateMenu() const
{
	int index = 1;
	system("cls");
	for (auto& option : this->options)
	{
		if (Menu::GetPos() == index)
		{
			fmt::print(fg(fmt::color::cyan), "{} - {}\n", index, option.first);
		}
		else
		{
			fmt::print("{} - {}\n", index, option.first);
		}
		index++;
	}
}

void Menu::ExecuteOption() const
{
	int index = 1;
	for (auto& option : this->options)
	{
		if (this->GetPos() == index)
		{
			system("cls");
			option.second();
		}
		index++;
	}
}


void Menu::Active()
{
	this->SetPos(1);
	this->UpdateMenu();

	this->bMenuEnabled = true;



	while (this->bMenuEnabled)
	{

		const int key = _getch();

		//up
		if (key == 72)
		{
			this->SetPos(this->GetPos() - 1);
		}

		//down
		else if (key == 80)
		{
			this->SetPos(this->GetPos() + 1);
		}

		//enter
		else if (key == 13)
		{
			this->ExecuteOption();
		}

		else
		{
			continue;
		}
		this->UpdateMenu();
	}
	system("cls");
}