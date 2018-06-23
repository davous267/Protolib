/*
ContainerWrapper is a templated class with an aim of simplifying operations with containers.
It provides some additional & more complex functionality to the underlying container.

(c) 2018 David Kutak
*/

#pragma once
#include <algorithm>
#include <functional>
#include <type_traits>
#include <numeric>
#include <set>
#include <map>
#include <vector>

namespace protolib
{
	template<typename TContainer>
	class ContainerWrapper
	{
	private:
		TContainer mContainer;
	public:
		using value_type = typename TContainer::value_type;
		using reference = typename TContainer::reference;
		using const_reference = typename TContainer::const_reference;
		using size_type = typename TContainer::size_type;
		using iterator = typename TContainer::iterator;
		using const_iterator = typename TContainer::const_iterator;

		// Constructors

		/**
		Default constructor. Calls default constructor of underlying container.
		*/
		ContainerWrapper() = default;

		/**
		Constructor initializing container with a range of values

		@param from start of the range (inclusive)
		@param to end of the range (inclusive)
		@param step determines the distance between consecutive values
		*/
		ContainerWrapper(value_type from, value_type to, value_type step)
		{
			addRange(from, to, step);
		}

		/**
		Constructor initializing container with a range of values 
		defined by two iterators

		@param begin begin iterator
		@param end end iterator
		*/
		template<typename TIter>
		ContainerWrapper(TIter begin, TIter end)
		{
			addRange(begin, end);
		}

		/**
		Constructor replacing default-initialized underlying container
		with container as an argument

		@param container new container
		*/
		ContainerWrapper(const TContainer& container)
		{
			setContainer(container);
		}

		// General functions

		/**
		Returns reference to underlying container

		@return reference to underlying container
		*/
		TContainer& getContainer()
		{
			return mContainer;
		}

		/**
		Returns constant reference to underlying container

		@return constant reference to underlying container
		*/
		const TContainer& getContainer() const
		{
			return mContainer;
		}

		/**
		Replaces underlying container with a new one

		@param container new container
		*/
		void setContainer(const TContainer& container)
		{
			mContainer = container;
		}

		/**
		Returns iterator to the beginning of the underlying container

		@return iterator to the beginning of the underlying container
		*/
		auto begin()
		{
			return mContainer.begin();
		}

		/**
		Returns past-the-end iterator of the underlying container

		@return past-the-end iterator of the underlying container
		*/
		auto end()
		{
			return mContainer.end();
		}

		/**
		Returns iterator to the beginning of the underlying container

		@return iterator to the beginning of the underlying container
		*/
		auto begin() const
		{
			return mContainer.begin();
		}

		/**
		Returns past-the-end iterator of the underlying container

		@return past-the-end iterator of the underlying container
		*/
		auto end() const
		{
			return mContainer.end();
		}

		/**
		Returns constant iterator to the beginning of the underlying container

		@return constant iterator to the beginning of the underlying container
		*/
		auto cbegin() const
		{
			return mContainer.cbegin();
		}

		/**
		Returns constant past-the-end iterator of the underlying container

		@return constant past-the-end iterator of the underlying container
		*/
		auto cend() const
		{
			return mContainer.cend();
		}

		/**
		Returns reverse iterator to the beginning of the underlying container

		@return reverse iterator to the beginning of the underlying container
		*/
		auto rbegin()
		{
			return mContainer.rbegin();
		}

		/**
		Returns reverse past-the-end iterator of the underlying container

		@return reverse past-the-end iterator of the underlying container
		*/
		auto rend()
		{
			return mContainer.rend();
		}

		/**
		Returns reverse iterator to the beginning of the underlying container

		@return reverse iterator to the beginning of the underlying container
		*/
		auto rbegin() const
		{
			return mContainer.rbegin();
		}

		/**
		Returns reverse past-the-end iterator of the underlying container

		@return reverse past-the-end iterator of the underlying container
		*/
		auto rend() const
		{
			return mContainer.rend();
		}

		/**
		Returns constant reverse iterator to the beginning of the underlying container

		@return constant reverse iterator to the beginning of the underlying container
		*/
		auto crbegin() const
		{
			return mContainer.crbegin();
		}

		/**
		Returns constant reverse past-the-end iterator of the underlying container

		@return constant reverse past-the-end iterator of the underlying container
		*/
		auto crend() const
		{
			return mContainer.crend();
		}

		/**
		Inserts value at the end of the container

		@param value value to insert
		@return iterator to the newly inserted element
		*/
		iterator insert(const_reference value)
		{
			return mContainer.insert(end(), value);
		}

		/**
		Calls operator[] on underlying container

		@param index index to access
		@return value at index
		*/
		template<typename TKey>
		auto& operator[](const TKey& index)
		{
			return mContainer[index];
		}

		/**
		Calls operator[] on underlying container

		@param index index to access
		@return value at index
		*/
		template<typename TKey>
		const auto& operator[](const TKey& index) const
		{
			return mContainer[index];
		}

		/**
		Removes element being pointed to by the iterator

		@param iterator iterator pointing to element to remove
		*/
		void erase(iterator iterator)
		{
			mContainer.erase(iterator);
		}

		/**
		Removes element being pointed to by the iterator

		@param iterator iterator pointing to element to remove
		*/
		void erase(const_iterator iterator)
		{
			mContainer.erase(iterator);
		}

		/**
		Removes all elements being equal to value

		@param value values to remove
		*/
		void erase(const_reference value)
		{
			auto it = std::remove(begin(), end(), value);
			mContainer = TContainer(begin(), it);
		}

		/**
		Finds an element in the container

		@param value value to find
		@return iterator to the element if found, end() iterator otherwise
		*/
		iterator find(const_reference value)
		{
			return std::find(begin(), end(), value);
		}

		/**
		Finds an element in the container

		@param value value to find
		@return iterator to the element if found, end() iterator otherwise
		*/
		const_iterator find(const_reference value) const
		{
			return std::find(cbegin(), cend(), value);
		}

		/**
		Appends a range of values defined by two iterators

		@param begin begin iterator
		@param end end iterator
		@return reference to this
		*/
		template<typename TIter>
		ContainerWrapper& addRange(TIter begin, TIter end)
		{
			while (begin != end)
			{
				insert(*begin++);
			}

			return *this;
		}

		/**
		Appends a range of values

		@param beginVal start of the range (inclusive)
		@param endVal end of the range (inclusive)
		@param step determines the distance between consecutive values
		@return reference to this
		*/
		ContainerWrapper& addRange(value_type beginVal, value_type endVal, value_type step)
		{
			for (value_type i = beginVal; i <= endVal; i += step)
			{
				insert(i);
			}

			return *this;
		}

		/**
		Appends another container to the underlying one

		@param other container to append
		@return reference to this
		*/
		ContainerWrapper& addRange(const ContainerWrapper& other)
		{
			addRange(other.cbegin(), other.cend());
			return *this;
		}

		/**
		Returns size of the container

		@return size of the container
		*/
		size_type size() const
		{
			return mContainer.size();
		}

		/**
		Checks whether the container is empty or not

		@return true if underlying container is empty, false otherwise
		*/
		bool empty() const
		{
			return mContainer.empty();
		}

		/**
		Empties container
		*/
		void clear()
		{
			mContainer.clear();
		}

		// Specialized functions

		/**
		Checks whether the underlying container is sorted or not

		@return true if container is sorted, false otherwise
		*/
		bool isSorted() const
		{
			return std::is_sorted(cbegin(), cend());
		}

		/**
		Returns sorted copy of the container

		@return sorted copy of the container
		*/
		ContainerWrapper getSorted() const
		{
			ContainerWrapper res = *this;
			std::sort(res.begin(), res.end());
			return res;
		}

		/**
		Returns copy of the container in which only
		elements fulfilling given predicate are included

		@param pred unary predicate returning true for elements which should be kept
		@return copy of the container with elements where pred(elem) == true
		*/
		template<typename UnPred>
		ContainerWrapper where(UnPred pred) const
		{
			ContainerWrapper res;

			for (const_reference el : mContainer)
			{
				if (pred(el))
				{
					res.insert(el);
				}
			}

			return res;
		}

		/**
		Returns new container where each element comes from applying
		given unary function to every element of an old container

		@param func unary function applied to elements of container
		@return ContainerWrapper with underlying container of type TContRes
		*/
		template<typename TRes, typename TContRes>
		auto map(const std::function<TRes(value_type)>& func) const
		{
			ContainerWrapper<TContRes> res;

			for (const_reference el : mContainer)
			{
				res.insert(func(el));
			}

			return res;
		}

		/**
		Accumulates elements of a container to a single value
		starting with the init element

		@param init initial value of the accumulation
		@param func binary function to be applied to the values being processed
		@return resulting value of accumulation
		*/
		template<typename TRes, typename BinFunc>
		TRes accumulateLeft(TRes init, BinFunc func) const
		{
			return std::accumulate(begin(), end(), init, func);
		}

		/**
		Accumulates elements of a container to a single value
		starting with the fin element

		@param fin initial value of the accumulation
		@param func binary function to be applied to the values being processed
		@return resulting value of accumulation
		*/
		template<typename TRes, typename BinFunc>
		TRes accumulateRight(TRes fin, BinFunc func) const
		{
			auto funcRev = [func](auto lhs, auto rhs) { return func(rhs, lhs); };

			return std::accumulate(rbegin(), rend(), fin, funcRev);
		}

		/**
		Checks how many elements fulfill given predicate

		@param pred unary predicate
		@return number of elements for which pred(elem) == true
		*/
		template<typename UnPred>
		size_type count(UnPred pred) const
		{
			return std::count_if(begin(), end(), pred);
		}

		/**
		Sums elements of a container
		
		@return sum of the elements in the container
		*/
		value_type sum() const
		{
			return accumulateLeft(0, [](auto fst, auto snd) { return fst + snd; });
		}

		/**
		Returns arithmetic mean of the elements of the container

		@return arithmetic mean
		*/
		template<typename TRes = value_type>
		TRes average() const
		{
			return static_cast<TRes>(sum()) / size();
		}

		/**
		Returns maximum value stored in the container

		@return maximum value
		*/
		value_type max() const
		{
			return *std::max_element(begin(), end());
		}

		/**
		Returns minimum value stored in the container

		@return minimum value
		*/
		value_type min() const
		{
			return *std::min_element(begin(), end());
		}

		/**
		Returns copy of the container with the elements 
		being reversed

		@return reversed copy of the container
		*/
		ContainerWrapper reverse() const
		{
			return ContainerWrapper(rbegin(), rend());
		}

		/**
		Returns copy of the container with some elements
		at the beginning skipped

		@param numOfElements number of elements to skip
		@return copy of the container with numOfElements at the beginning skipped
		*/
		ContainerWrapper skip(size_t numOfElements) const
		{
			const_iterator iter = cbegin();
			if (numOfElements <= size())
			{
				for (size_t i = 0; i < numOfElements; ++i) { ++iter; }
				return ContainerWrapper(iter, cend());
			}
			return ContainerWrapper();
		}

		/**
		Returns copy of the container where elements
		at the beginning are skipped as long as the 
		predicate in the argument returns true

		@param pred unary predicate determining "skipping criteria"
		@return copy of the container with some of the initial elements skipped
		*/
		template<typename UnPred>
		ContainerWrapper skipWhile(UnPred pred) const
		{
			ContainerWrapper result;

			bool shouldSkip = true;
			for (const_reference el : mContainer)
			{
				if (!shouldSkip || !pred(el))
				{
					result.insert(el);
					shouldSkip = false;
				}
			}

			return result;
		}

		/**
		Returns copy of the container containing only some
		of the values at the beginning

		@param numOfElements number of elements to take
		@return copy of the container with numOfElements taken
		*/
		ContainerWrapper take(size_t numOfElements) const
		{
			const_iterator iter = cbegin();
			for (size_t i = 0; i < numOfElements && iter != cend(); ++i, ++iter);
			return ContainerWrapper(cbegin(), iter);
		}

		/**
		Returns copy of the container where elements
		at the beginning are included as long as the
		predicate in the argument returns true
		When predicate returns false, all elements 
		>= current position are discarded.

		@param pred unary predicate determining "including criteria"
		@return copy of the container with some of the initial elements skipped
		*/
		template<typename UnPred>
		ContainerWrapper takeWhile(UnPred pred) const
		{
			ContainerWrapper result;

			for (const_reference el : mContainer)
			{
				if (pred(el))
				{
					result.insert(el);
				}
				else { break; }
			}

			return result;
		}

		/**
		Returns copy of the container containing only unique elements

		@return copy of the container where each element is included only once
		*/
		ContainerWrapper unique() const
		{
			std::set<value_type> foundElements;
			ContainerWrapper result;

			for (const_reference el : mContainer)
			{
				if (foundElements.find(el) == foundElements.end())
				{
					result.insert(el);
					foundElements.insert(el);
				}
			}

			return result;
		}

		/**
		Groups elements according to their output when passed 
		to given unary function

		@param func unary function to determine elements' groups
		@return map where key contains obtained output of unary function 
		        for elements stored as a value of this map item
		*/
		template<typename UnFunc>
		auto groupBy(UnFunc func) const
		{
			static_assert(std::is_default_constructible<value_type>::value,
				"Data type of values stored in container must be default constructible to use groupBy member function.");
			std::map<decltype(func(value_type())), std::vector<value_type>> result;

			for (const_reference el : mContainer)
			{
				auto key = func(el);
				auto it = result.find(key);
				if (it != result.end())
				{
					it->second.push_back(el);
				}
				else
				{
					result.insert(std::make_pair(key, std::vector<value_type>{ el }));
				}
			}

			return result;
		}
	};

	template<typename TContainer>
	std::ostream& operator<<(std::ostream& os, const ContainerWrapper<TContainer>& container)
	{
		for (auto el : container)
		{
			os << el << "; ";
		}
		return os;
	}

	template<typename TContainer>
	bool operator==(const ContainerWrapper<TContainer>& lhs,
		const ContainerWrapper<TContainer>& rhs)
	{
		return lhs.getContainer() == rhs.getContainer();
	}

	template<typename TContainer>
	bool operator!=(const ContainerWrapper<TContainer>& lhs,
		const ContainerWrapper<TContainer>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename TContainer>
	bool operator<(const ContainerWrapper<TContainer >& lhs,
		const ContainerWrapper<TContainer>& rhs)
	{
		return lhs.getContainer() < rhs.getContainer();
	}

	template<typename TContainer>
	bool operator<=(const ContainerWrapper<TContainer>& lhs,
		const ContainerWrapper<TContainer>& rhs)
	{
		return !(lhs > rhs);
	}

	template<typename TContainer>
	bool operator>(const ContainerWrapper<TContainer>& lhs,
		const ContainerWrapper<TContainer>& rhs)
	{
		return lhs.getContainer() > rhs.getContainer();
	}

	template<typename TContainer>
	bool operator>=(const ContainerWrapper<TContainer>& lhs,
		const ContainerWrapper<TContainer>& rhs)
	{
		return !(lhs < rhs);
	}
}