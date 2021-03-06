#ifndef SOLAIRE_NEW_ALLOCATOR_HPP
#define SOLAIRE_NEW_ALLOCATOR_HPP

//Copyright 2015 Adam Smith
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

// Contact :
// Email             : solairelibrary@mail.com
// GitHub repository : https://github.com/SolaireLibrary/SolaireCPP

#include "Solaire\Core\Allocator.hpp"
#include <map>

namespace Solaire{

	class NewAllocator : public Allocator {
	private:
		std::map<const void*, uint32_t> mAllocations;
		uint32_t mAllocatedBytes;
	public:
		NewAllocator() :
			mAllocatedBytes(0)
		{}

		SOLAIRE_EXPORT_CALL ~NewAllocator() throw() {
			deallocateAll();
		}

		// Inherited from Allocator

		uint32_t SOLAIRE_EXPORT_CALL getAllocatedBytes() const throw()  override {
			return mAllocatedBytes;
		}

		uint32_t SOLAIRE_EXPORT_CALL getFreeBytes() const throw() override {
			return UINT32_MAX - mAllocatedBytes;
		}

		uint32_t SOLAIRE_EXPORT_CALL sizeOf(const void* const aObject) throw() override {
			auto it = mAllocations.find(aObject);
			if(it == mAllocations.end()) return 0;
			return it->second;
		}

		void* SOLAIRE_EXPORT_CALL allocate(const size_t aBytes) throw() override {
			void* const tmp = operator new(aBytes);
			mAllocations.emplace(tmp, aBytes);
			mAllocatedBytes += aBytes;
			return tmp;
		}

		bool SOLAIRE_EXPORT_CALL deallocate(const void* const aObject) throw() override {
			auto it = mAllocations.find(aObject);
			if(it == mAllocations.end()) return false;
			operator delete(const_cast<void*>(aObject));
			mAllocatedBytes -= it->second;
			mAllocations.erase(it);
			return true;
		}

		bool SOLAIRE_EXPORT_CALL deallocateAll() throw() {
			for(auto i : mAllocations) {
				operator delete(const_cast<void*>(i.first));
			}
			mAllocatedBytes = 0;
			return true;
		}
	};

}
#endif
