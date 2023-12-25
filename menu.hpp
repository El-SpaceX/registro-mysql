#include <fmt/color.h>
#include <cstdint>
#include <unordered_map>
#include <conio.h>

class Menu
{
private:
	//option | callback
	uint8_t iCurrentPos = 1, iMaxPos = 0;
	bool bMenuEnabled{};

protected:
	std::unordered_map<std::string, std::function<void()>> options;
	inline void SetMaxPos(const size_t max_pos)
	{
		this->iMaxPos = static_cast<uint8_t>(max_pos);
	}

	inline void SetPos(const uint8_t pos)
	{
		if (pos > this->iMaxPos)
		{
			this->iCurrentPos = 1;
		}
		else if (pos < 1)
		{
			this->iCurrentPos = this->iMaxPos;
		}
		else
		{
			this->iCurrentPos = pos;
		}
	}

	inline uint8_t GetPos() const
	{
		return this->iCurrentPos;
	}

public:
	inline void AddOption(const std::string& option_name, const std::function<void()> callback)
	{
		this->options.insert(std::make_pair(option_name, callback));
		this->SetMaxPos(options.size());
	}

	inline void RemoveOption(const std::string& option_name)
	{
		this->options.erase(option_name);
		this->SetMaxPos(options.size());
	}

	inline void StopMenu()
	{
		system("cls");
		this->bMenuEnabled = false;
	}


	void Active();
	void UpdateMenu() const;
	void ExecuteOption() const;
};