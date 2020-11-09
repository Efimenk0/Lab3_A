#include "TernaryVectorClass.h"




namespace TernaryVectorClass {


	/* --------------------------------------------------------- */
	/*				    	  Конструкторы					 	 */
	/* --------------------------------------------------------- */


	// Конструктор с инициализацией заданным кол-вом неопределенных значений
	TernaryVector::TernaryVector(int X) {
		if (X > SZ)
            throw std::exception("Кол-во неопреденных значений не может быть больше максимального размера вектора. Попроюуйте заново.\n");
		if (X < 0)
			throw std::exception("Кол-во неопреденных значений не может быть отрицательным. Попробуйте еще раз.\n");
		for (int i = 0; i < X; ++i)
			vector[i] = 'X';
		top = X;
	}


	// Конструктор с инициализацией значениями элементов вектора как строки символов
	TernaryVector::TernaryVector(const std::string& str) {
		// Проверка на пробелы нужна, если мы решим воспользоваться явным преобразованием типа через конструктор и применим ее к "неправильной" строке
		int size = 0, str_i = 0;
		// Ищем размер строки (до конца строки, либо до певого пробела/табуляции)
		while (str[str_i] != '\0' && str[str_i] != ' ' && str[str_i] != '\t') {
			if (str[str_i] != ' ' && str[str_i] != '\t')
				size++;
			++str_i;
		}
		if (size > SZ)
			throw std::exception("Размер строки превышает максимальный размер вектора. Попробуйте еще раз.\n");
		str_i = 0;
		top = size;
		int i = 0;
		while (i < size) {
			while (str[str_i] == ' ' || str[str_i] == '\t')
				str_i++;
			if (str[str_i] == '0')
				vector[i] = '0';
			else if (str[str_i] == '1')
				vector[i] = '1';
			else
				vector[i] = 'X';
			str_i++;
			i++;
		}
	}


	/* --------------------------------------------------------- */
	/*				      Сеттеры - set'теры					 */
	/* --------------------------------------------------------- */

		// Инвертирование вектора
	TernaryVector& TernaryVector::Invertion() {
		for (int i = 0; i < SZ; ++i)
			if (vector[i] == '1')
				vector[i] = '0';
			else if (vector[i] == '0')
				vector[i] = '1';
		return *this;
	}


	// Выполнение порязрядной конъюнкции данного вектора с другим (Унарная операция)
	TernaryVector& TernaryVector::Conjunction(const TernaryVector& con) {
		int corrent_size;
		if (top <= con.top)
			corrent_size = top;
		else
			corrent_size = con.top;
		for (int i = 0; i < corrent_size; ++i)
			if (vector[i] + con.vector[i] == 98)
				vector[i] = '1';
			else if (vector[i] + con.vector[i] == 137 || (vector[i] + con.vector[i]) == 176)
				vector[i] = 'X';
			else
				vector[i] = '0';
		for (int i = corrent_size; i < top; i++)
			if (vector[i] == '1')
				vector[i] = 'X';
		return *this;
	}


	/* --------------------------------------------------------- */
	/*						 Другие методы						 */
	/* --------------------------------------------------------- */


	// Выполнение порязрядной дизъюнкции данного вектора с другим (Бинарная операция)
	const TernaryVector Disjunction(const TernaryVector& left, const TernaryVector& right) {
		TernaryVectorClass::TernaryVector tmp;
		int corrent_size;
		if (left.top <= right.top) {
			corrent_size = left.top;
			tmp.top = right.top;
		}
		else {
			corrent_size = right.top;
			tmp.top = left.top;
		}
		for (int i = 0; i < corrent_size; ++i)
			if ((left.vector[i] + right.vector[i]) == 96)
				tmp.vector[i] = '0';
			else if ((left.vector[i] + right.vector[i]) == 136 || (left.vector[i] + right.vector[i]) == 176)
				tmp.vector[i] = 'X';
			else
				tmp.vector[i] = '1';
		int max_size;
		if (corrent_size == right.top)
			for (int i = corrent_size; i < left.top; i++) {
				if (left.vector[i] != '1')
					tmp.vector[i] = 'X';
				else
					tmp.vector[i] = left.vector[i];
			}
		else
			for (int i = corrent_size; i < right.top; i++) {
				if (right.vector[i] != '1')
					tmp.vector[i] = 'X';
				else
					tmp.vector[i] = right.vector[i];
			}
		return tmp;
	}


	// Сравнение двух векторов (Возвращает индекс первого несовпадения, либо -1, если вектора совпадают и -2, если размеры векторов разные (сравнивать незачем))
	const int Compare(const TernaryVector& left, const TernaryVector& right) {
		if (left.top != right.top)
			return -1;				// Код "-1" - разные размеры векторов
		for (int i = 0; i < left.top; ++i)
			if (left.vector[i] != right.vector[i])
				return i;
		return -2;					// Код "-2" - вектора совпадают
	}



	// Анализ неопределенности кода - возвращает индекс первой встреченной неопределенности, либо -1, если ее нет
	int TernaryVector::Analysis() const {
		for (int i = 0; i < top; ++i)
			if (vector[i] == 'X')
				return i;
		return -1;
	}


	/* --------------------------------------------------------- */
	/*						 Друзья класса 						 */
	/* --------------------------------------------------------- */


	// Вввод состояния вектора из входного потока (с возможным setstate)
	std::istream& operator >> (std::istream& stream, TernaryVector& vector) {
		std::string str;
		stream >> str;
		if (str.size() <= vector.SZ) {
			vector.top = str.size();
			for (int i = 0; i < str.size(); ++i)
				if (str[i] == '0')
					vector.vector[i] = '0';
				else if (str[i] == '1')
					vector.vector[i] = '1';
				else
					vector.vector[i] = 'X';
		}
		else
			stream.setstate(std::ios::failbit);	// Было введено слово, большее по размеру, чем максимальный размер вектора
		return stream;
	}



	// Вывод текущего вектора в выходной поток
	std::ostream& operator << (std::ostream& stream, const TernaryVector& vector) {
		if (vector.top > 0) {
			for (int i = 0; i < 2 * vector.top + 1; ++i)
				stream << "-";
			stream << '\n';
			for (int i = 0; i < vector.top; ++i)
				stream << "|" << vector.vector[i];
			stream << "|\n";
			for (int i = 0; i < 2 * vector.top + 1; ++i)
				stream << "-";
			stream << '\n';
		}
		else
			stream << "Вектор пуст.\n";
		return stream;
	}


}
