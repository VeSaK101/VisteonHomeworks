#include "pch.h"
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <vector>
#include <tuple>

using std::string;
using TabContainer = std::list<std::tuple<string, string, time_t>>;
using TabContainerIterator = std::list<std::tuple<string, string, time_t>>::iterator;
using TabType = std::tuple<string, string, time_t>;
 

void loadFile(string &url, TabType &tab)
{
	string line;
	std::ifstream tabContent("content/" + url + ".txt");
	if (tabContent.is_open())
	{
		while (getline(tabContent, line))
		{
			std::get<1>(tab) += (line + '\n');
		}
	}
}

void setTabInfo(TabType &tab, string adress)
{
	std::get<0>(tab) = adress;
	std::get<1>(tab) = "";
	std::get<2>(tab) = time(NULL);

	loadFile(adress, tab);
}

TabType createTab(string adress)
{
	TabType tab;
	setTabInfo(tab, adress);

	return tab;
}

void goToURL(string &adress, TabContainerIterator &currentTab)
{
	setTabInfo((*currentTab), adress);
}

void insertTab(string &adress, TabContainerIterator &currentTab, TabContainer &openTabs)
{
	for (TabContainerIterator tab = openTabs.begin(); tab != openTabs.end(); tab++)
	{
		if (std::get<0>(*tab) == adress)
		{
			currentTab = tab;
			return;
		}
	}
	auto tab = createTab(adress);
	TabContainerIterator it = currentTab;
	std::advance(it, 1);
	openTabs.insert(it, tab);
	currentTab = std::next(currentTab);
}

void goBack(TabContainerIterator &currentTab, TabContainer &openTabs)
{

	if (std::prev(currentTab) != openTabs.end())
	{
		currentTab = std::prev(currentTab);
	}
}

void goForward(TabContainerIterator &currentTab, TabContainer &openTabs)
{
	if (std::next(currentTab) != openTabs.end())
	{
		currentTab = std::next(currentTab);
	}
}

void removeTab(TabContainerIterator &currentTab, TabContainer &openTabs)
{
	TabContainerIterator prev;
	if (currentTab != openTabs.begin())
	{
		TabContainerIterator prev = std::prev(currentTab);
	}
	else
	{
		prev = openTabs.end();
	}

	TabContainerIterator next = openTabs.erase(currentTab);

	if (next != openTabs.end())
	{
		currentTab = next;
		return;
	}
	if (prev != openTabs.end())
	{
		currentTab = prev;
		return;
	}
	if (currentTab == openTabs.end())
	{
		auto blank = createTab("about:blank");
		openTabs.push_back(blank);
		currentTab = openTabs.begin();
	}
}
void print(TabContainerIterator &currentTab, TabContainer &openTabs)
{
	for (TabContainerIterator tab = openTabs.begin(); tab != openTabs.end(); tab++)
	{
		if (currentTab == tab)
		{
			std::cout << "> " << std::get<0>(*tab) << " " << std::get<2>(*tab) << std::endl;
		}
		else
		{
			std::cout << std::get<0>(*tab) << " " << std::get<2>(*tab) << std::endl;
		}

	}

}

bool sortByURL(const std::tuple<string, string, time_t> &a,
	const std::tuple<string, string, time_t> &b)
{
	if (std::get<0>(a) == std::get<0>(b))
	{
		return (std::get<2>(a) < std::get<2>(b));
	}
		
	return std::get<0>(a) < std::get<0>(b);
	
}
bool sortByTimestamp(const std::tuple<string, string, time_t> &a,
	const std::tuple<string, string, time_t> &b)
{
	if (std::get<2>(a) == std::get<2>(b))
	{
		return (std::get<0>(a) < std::get<0>(b));
	}
		
	return std::get<2>(a) < std::get<2>(b);
	
}

void sort(string comparer, TabContainer &openTabs)
{
	std::sort(openTabs.begin(), openTabs.end(), sortByURL);
}

int main() {

	TabContainer openTabs;
	auto blank = createTab("about:blank");
	openTabs.push_back(blank);

	TabContainerIterator current = openTabs.begin();


	while (true)
	{
		std::vector<string> arguments;

		string line;
		std::getline(std::cin, line);

		std::stringstream stream(line);
		string token;

		while (std::getline(stream, token, ' ')) {
			arguments.push_back(token);
		}

		if (arguments.size() != 0)
		{
			if (arguments[0] == "GO")
			{
				goToURL(arguments[1], current);
			}
			if (arguments[0] == "INSERT")
			{
				insertTab(arguments[1], current, openTabs);
			}
			if (arguments[0] == "BACK")
			{
				goBack(current, openTabs);
			}
			if (arguments[0] == "FORWARD")
			{
				goForward(current, openTabs);
			}
			if (arguments[0] == "REMOVE")
			{
				removeTab(current, openTabs);
			}
			if (arguments[0] == "PRINT")
			{
				print(current, openTabs);
			}
			if (arguments[0] == "SORT")
			{
				sort(arguments[1], openTabs);
			}

		}

	}

	return 0;
}


