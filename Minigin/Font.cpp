#include "MiniginPCH.h"
#include "Font.h"
#include "Logger.h"

TTF_Font* dae::Font::GetFont() const 
{
	return m_Font;
}

dae::Font::Font(const std::string& fullPath, unsigned int size) 
	: m_Font(nullptr)
	, m_Size(size)
{
	m_Font = TTF_OpenFont(fullPath.c_str(), size);
	if (m_Font == nullptr) 
	{
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::FAIL, std::string("Failed to load font: ") + SDL_GetError(), this });
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}
}

dae::Font::~Font()
{
	TTF_CloseFont(m_Font);
}