#include <vector>


template <class T, class Compare >
void mergeSort(std::vector<T>& list, Compare comp);

template <class T, class Compare >
void mergeSortHelper(std::vector<T>& list, Compare comp, int start, int end);

template <class T, class Compare >
void merge(std::vector<T>& list, Compare comp, int start, int mid, int end);

template <class T, class Compare >
void mergeSort(std::vector<T>& list, Compare comp){
	// if list contains one element
	if(list.size() <= 1){
		return;
	}
	// if list contains two elements
	else if(list.size() == 2){
		if(comp(list[1], list[0])){
			T temp = list[0];
			list[0] = list[1];
			list[1] = temp;
			return;
		}
	}
	// initiate recursive merge sort
	else{
		std::vector<T> other(list);
		T blank = list[0];
		
		other.push_back(blank);
		mergeSortHelper(other, comp, 0, list.size());

		typename std::vector<T>::iterator it;
		int count = 0;
		for(it = other.begin(); it != other.end(); ++it){
			if(*it == blank){
				if(count == 0){
					other.erase(it);
				}
				count++;
			}
		}

		for(unsigned int i = 0; i < list.size(); i++){
			list[i] = other[i];
		}
	}
}

template <class T, class Compare >
void mergeSortHelper(std::vector<T>& list, Compare comp, int start, int end){
	if(start >= end){
		return;
	}
	int mid = (start + end) / 2;
	// perform merge sort on split array
	mergeSortHelper(list, comp, start, mid);
	mergeSortHelper(list, comp, mid + 1, end);
	// merge both parts of array
	merge(list, comp, start, mid, end);
}

template <class T, class Compare >
void merge(std::vector<T>& list, Compare comp, int p, int q, int r){
	
	// calculate size of left and right array
	int size1 = q - p + 1;
	int size2 = r - q;
	
	// create left and right array
	std::vector <T> L;
	std::vector<T> R;

	// copy contents from list into L and R
	for(int i = 0; i < size1; i++){
		L.push_back(list[p + i]);
	}

	for(int j = 0; j < size2; j++){
		R.push_back(list[q + j + 1]);
	}

	// create iterators for left, right, and main arrays
	int i = 0;
	int j = 0;
	int k;

	for(k = p; k <= r && i < size1 && j < size2; k++){
		// walk down left and right arrays and adjust main array
		if(comp(L[i], R[j])){
			list[k] = L[i];
			i++;
		}
		else{
			list[k] = R[j];
			j++;
		}
	}

	// fill in remaining elements
	for(i = i; i < size1; ++i){
		list[k] = L[i];
		k++;
	}

	// fill in remaining elements
	for(j = j; j < size2; ++j){
		list[k] = R[j];
		k++;
	}

}
