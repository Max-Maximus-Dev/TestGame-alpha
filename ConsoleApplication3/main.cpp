#include <iostream>
#include <string>
#include <vector>
#include "Exceptions.h"
#include "LogicOfProgarm.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

namespace Menu {
	namespace GuestMenu {
		void GuestMenu(Guest* guest, std::vector<categoryOfTests*>& categories) {
			std::cout << "Welcome, " << guest->getLogin() << "!\n";
			std::cout << "1. Pass Test\n";
			std::cout << "2. View Grades\n";
			std::cout << "3. Logout\n";
			int choice;
			std::cin >> choice;
			switch (choice) {
			case 1: {
				std::cout << "\n-- Select a category --\n";
				for (int i = 0; i < categories.size(); i++) {
					std::cout << i + 1 << ". " << categories[i]->getCategoryName() << std::endl;
				}
				std::cout << "Enter category number: ";
				int category_choice;
				std::cin >> category_choice;
				std::cin.ignore();
				if (category_choice < 1 || category_choice > categories.size()) {
					std::cerr << "Invalid category choice.\n";
					return;
				}
				categoryOfTests* selected_category = categories[category_choice - 1];
				std::cout << "\n-- Select a test --\n";
				auto tests = selected_category->getTests();
				for (int i = 0; i < tests.size(); i++) {
					std::cout << i + 1 << ". " << tests[i]->getTestName() << std::endl;
				}
				std::cout << "Enter test number: ";
				int test_choice;
				std::cin >> test_choice;
				if (test_choice < 1 || test_choice > tests.size()) {
					std::cerr << "Invalid test choice.\n";
					return;
				}
				Test* selected_test = tests[test_choice - 1];
				passTest(guest, selected_test);
				break;
			}
			case 2: {
				std::cout << "\n-- Your Grades --\n";
				auto grades = guest->getGrades();
				if (grades.empty()) {
					std::cout << "No grades available.\n";
				}
				else {
					for (int i = 0; i < grades.size(); i++) {
						std::cout << "Test " << i + 1 << ": " << grades[i] << std::endl;
					}
				}
				break;
			}
			case 3: {
				std::cout << "Logging out...\n";
				return;
			}
			default: {
				std::cerr << "Invalid choice.\n";
				break;
			}
			}
		}
	}
	namespace AdminMenu {
		void AdminMenu(Administrator* admin, std::vector<User*>& users, std::vector<categoryOfTests*>& categories, std::vector<Test*>& all_tests) {
			std::cout << "Welcome, Administrator " << admin->getLogin() << "!\n";
			std::cout << "1. Create Guest Account\n";
			std::cout << "2. Delete Guest Account\n";
			std::cout << "3. Add category of tests\n";
			std::cout << "4. Delete category of tests\n";
			std::cout << "5. Add test\n";
			std::cout << "6. Delete test\n";
			std::cout << "7. Add question to test\n";
			std::cout << "8. Delete question from test\n";
			std::cout << "9. Manage login/password of guest\n";
			std::cout << "10. View Statistics\n";
			int choice;
			std::cin >> choice;
			switch (choice) {
			case 1: {
				std::string login, password;
				std::cout << "Enter guest login: ";
				std::cin >> login;
				std::cout << "Enter guest password: ";
				std::cin >> password;
				createGuestAccount(admin, users, login, password);
				SerializeData(users, categories);
				break;
			}
			case 2: {
				std::string login;
				std::cout << "Enter guest login to delete: ";
				std::cin >> login;
				deleteGuestAccount(admin, users, login);
				SerializeData(users, categories);
				break;
			}
			case 3: {
				std::string category_name;
				std::cout << "Enter category name: ";
				std::cin >> category_name;
				categoryOfTests* new_category = new categoryOfTests(category_name);
				categories.push_back(new_category);
				SerializeData(users, categories);
				std::cout << "Category added successfully.\n";
				break;
			}
			case 4: {
				std::string category_name;
				std::cout << "Enter category name to delete: ";
				std::cin >> category_name;
				deleteCategoryOfTests(admin, categories, category_name);
				SerializeData(users, categories);
				break;
			}
			case 5: {
				std::string category_name, test_name;
				std::cout << "Enter category name to add test to: ";
				std::cin >> category_name;
				categoryOfTests* category = nullptr;
				for (auto cat : categories) {
					if (cat->getCategoryName() == category_name) {
						category = cat;
						break;
					}
				}
				if (!category) {
					std::cerr << "Category not found.\n";
					return;
				}
				std::cout << "Enter test name: ";
				std::cin >> test_name;
				addTest(admin, category, test_name);
				SerializeData(users, categories);
				break;
			}
			case 6: {
				std::string category_name, test_name;
				std::cout << "Enter category name to delete test from: ";
				std::cin >> category_name;
				categoryOfTests* category = nullptr;
				for (auto cat : categories) {
					if (cat->getCategoryName() == category_name) {
						category = cat;
						break;
					}
				}
				if (!category) {
					std::cerr << "Category not found.\n";
					return;
				}
				std::cout << "Enter test name to delete: ";
				std::cin >> test_name;
				deleteTest(admin, category, test_name);
				SerializeData(users, categories);
				break;
			}
			case 7: {
				std::string test_name, question_text;
				std::cout << "Enter category name of the test: ";
				std::string category_name;
				std::cin >> category_name;
				categoryOfTests* category = nullptr;
				for (auto cat : categories) {
					if (cat->getCategoryName() == category_name) {
						category = cat;
						break;
					}
				}
				if (!category) {
					std::cerr << "Category not found.\n";
					return;
				}
				std::cout << "Enter test name to add question to: ";
				std::cin >> test_name;
				Test* test = nullptr;
				for (auto t : category->getTests()) {
					if (t->getTestName() == test_name) {
						test = t;
						break;
					}
				}
				if (!test) {
					std::cerr << "Test not found.\n";
					return;
				}
				std::cout << "Enter question text to add: ";
				std::cin.ignore();
				getline(std::cin, question_text);
				std::vector<Answer> answers;
				int num_answers;
				std::cout << "Enter number of answers: ";
				std::cin >> num_answers;
				for (int i = 0; i < num_answers; i++) {
					std::string answer_text;
					bool is_correct;
					std::cout << "Enter text for answer " << i + 1 << ": ";
					std::cin.ignore();
					getline(std::cin, answer_text);
					std::cout << "Is this answer correct? (1 for yes, 0 for no): ";
					std::cin >> is_correct;
					answers.emplace_back(answer_text, is_correct);
				}
				addQuestionToTest(admin, test, question_text, category, answers);
				SerializeData(users, categories);
				break;
			}
			case 8: {
				std::string test_name, question_text;
				std::cout << "Enter category name of the test: ";
				std::string category_name;
				std::cin >> category_name;
				categoryOfTests* category = nullptr;
				for (auto cat : categories) {
					if (cat->getCategoryName() == category_name) {
						category = cat;
						break;
					}
				}
				if (!category) {
					std::cerr << "Category not found.\n";
					return;
				}
				std::cout << "Enter test name to delete question from: ";
				std::cin >> test_name;
				Test* test = nullptr;
				for (auto t : category->getTests()) {
					if (t->getTestName() == test_name) {
						test = t;
						break;
					}
				}
				if (!test) {
					std::cerr << "Test not found.\n";
					return;
				}
				std::cout << "Enter question text to delete: ";
				std::cin >> question_text;
				deleteQuestionFromTest(admin, test, question_text, category);
				SerializeData(users, categories);
				break;
			}
			case 9: {
				std::string login;
				std::cout << "Enter guest login to manage: ";
				std::cin >> login;
				manageLoginPasswordGuest(admin, users, login);
				SerializeData(users, categories);
				break;
			}
			case 10: {
				showStatistics(admin, users, categories, all_tests);
				break;
			}
			default: {
				std::cerr << "Invalid choice.\n";
				break;
			}
			}
		}
	}
}
extern std::vector<User*> users;
extern std::vector<categoryOfTests*> categories;
extern std::vector<Test*> all_tests;
int main()
{
	if (!fs::exists("DataBase")) {
		fs::create_directory("DataBase");
	}
	int admin_count = 0;
	try {
		DeserializeData(users, categories);
	}
	catch (const dataDeserializationError& e) {
		std::cerr << e.what() << " Starting with empty data.\n";
	}
	while (true) {
		std::cout << "\n-- Main Menu --\n";
		std::cout << "1. Register\n";
		std::cout << "2. Login\n";
		std::cout << "3. Exit\n";
		int choice;
		std::cin >> choice;
		if (choice == 1) {
			std::string login, password, role;
			std::cout << "Enter login: ";
			std::cin >> login;
			std::cout << "Enter password: ";
			std::cin >> password;
			std::cout << "Enter role (Administrator/Guest): ";
			std::cin >> role;
			registerUser(login, password, role);
			SerializeData(users, categories);
		}
		else if (choice == 2) {
			std::string login, password;
			std::cout << "Enter login: ";
			std::cin >> login;
			std::cout << "Enter password: ";
			std::cin >> password;
			User* user = loginUser(login, password);
			if (user) {
				if (user->getRole() == "Administrator") {
					Administrator* admin = dynamic_cast<Administrator*>(user);
					Menu::AdminMenu::AdminMenu(admin, users, categories, all_tests);
				}
				else if (user->getRole() == "Guest") {
					Guest* guest = dynamic_cast<Guest*>(user);
					Menu::GuestMenu::GuestMenu(guest, categories);
				}
			}
		}
		else if (choice == 3) {
			std::cout << "Exiting program...\n";
			break;
		}
		else {
			std::cerr << "Invalid choice.\n";
			std::cin.clear();
			std::cin.ignore();
		}
	}
	try {
		SerializeData(users, categories);
	}
	catch (const dataSerializationError& e) {
		std::cerr << e.what() << " Data not saved.\n";
	}
	for (auto user : users) {
		delete user;
	}
	for (auto category : categories) {
		delete category;
	}
}

