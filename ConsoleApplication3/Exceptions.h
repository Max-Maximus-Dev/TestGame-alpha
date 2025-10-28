#pragma once
#include <iostream>
#include <exception>
class theMaximumNumberOfAdminsIsExceeded : public std::exception {
public:
	const char* what() const noexcept override {
		return "The maximum number of administrators is exceeded.";
	}
};
class userAlreadyExists : public std::exception {
public:
	const char* what() const noexcept override {
		return "User already exists.";
	}
};
class userNotFound : public std::exception {
public:
	const char* what() const noexcept override {
		return "User not found.";
	}
};
class invalidCredentials : public std::exception {
public:
	const char* what() const noexcept override {
		return "Invalid credentials.";
	}
};
class testNotFound : public std::exception {
public:
	const char* what() const noexcept override {
		return "Test not found.";
	}
};
class questionNotFound : public std::exception {
public:
	const char* what() const noexcept override {
		return "Question not found.";
	}
};
class categoryNotFound : public std::exception {
public:
	const char* what() const noexcept override {
		return "Category not found.";
	}
};
class invalidInput : public std::exception {
public:
	const char* what() const noexcept override {
		return "Invalid input.";
	}
};
class dataSerializationError : public std::exception {
public:
	const char* what() const noexcept override {
		return "Data serialization error.";
	}
};
class dataDeserializationError : public std::exception {
public:
	const char* what() const noexcept override {
		return "Data deserialization error.";
	}
};
class testAlreadyExists : public std::exception {
public:
	const char* what() const noexcept override {
		return "Test already exists.";
	}
};
class categoryAlreadyExists : public std::exception {
public:
	const char* what() const noexcept override {
		return "Category already exists.";
	}
};
class questionAlreadyExists : public std::exception {
public:
	const char* what() const noexcept override {
		return "Question already exists.";
	}
};
class answerAlreadyExists : public std::exception {
	public:
	const char* what() const noexcept override {
		return "Answer already exists.";
	}
};

