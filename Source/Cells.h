#pragma once
#include <vector>

template<typename T>
class Cells
{
public:
	Cells(int width, int height) :
		m_width{ width },
		m_height{ height }
	{
		m_data.resize(width * height);
	}

	const T& Read(int x, int y) const { return m_data[x + (y * m_width)]; }
	void Write(int x, int y, const T& value) { m_data[x + (y * m_width)] = value; }

public:
	int m_width = 0;
	int m_height = 0;

	std::vector<T> m_data;
};