#ifndef SOLAIRE_INTEGER_CONTAINER_HPP
#define SOLAIRE_INTEGER_CONTAINER_HPP

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

/*!
	\file IntegerContainer.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\date
	Created			: 13th January 2016
	Last Modified	: 13th January 2016
*/

#include <type_traits>

namespace Solaire {

    namespace Implementation {
        template<class T, const bool SIGN, typename ENABLE = void>
        struct BiggerIntegerStruct {
            typedef void Type;
        };

        #define SOLAIRE_BIGGER_INTEGER_STRUCT(aType, aSignedType, aUnsignedType)\
        template<class T, const bool SIGN>\
        struct BiggerIntegerStruct<T, SIGN, typename std::enable_if<std::is_same<T, aType>::value && ! SIGN>::type> {\
            typedef aUnsignedType Type;\
        };\
        template<class T, const bool SIGN>\
        struct BiggerIntegerStruct<T, SIGN, typename std::enable_if<std::is_same<T, aType>::value && SIGN>::type> {\
            typedef aSignedType Type;\
        };

        SOLAIRE_BIGGER_INTEGER_STRUCT(uint8_t,  uint16_t,   int16_t);
        SOLAIRE_BIGGER_INTEGER_STRUCT(uint16_t, uint32_t,   int32_t);
        SOLAIRE_BIGGER_INTEGER_STRUCT(uint32_t, uint64_t,   int64_t);
        SOLAIRE_BIGGER_INTEGER_STRUCT(uint64_t, uint64_t,   int64_t);
        SOLAIRE_BIGGER_INTEGER_STRUCT(int8_t,   uint8_t,    int16_t);
        SOLAIRE_BIGGER_INTEGER_STRUCT(int16_t,  uint16_t,   int32_t);
        SOLAIRE_BIGGER_INTEGER_STRUCT(int32_t,  uint32_t,   int64_t);
        SOLAIRE_BIGGER_INTEGER_STRUCT(int64_t,  uint64_t,   int64_t);

        #undef SOLAIRE_BIGGER_INTEGER_STRUCT
    }

    /*!
        \brief An integer type that has a higher maximum value than type T.
        \tparam T The base integer type.
        \tparam SIGN True if the defined type should be signed, false for unsigned. This defaults to the signedness of T.
    */
    template<class T, const bool SIGN = std::is_signed<T>::value>
    using BiggerInteger = typename Implementation::BiggerIntegerStruct<T, SIGN>::Type;

    namespace Implementation {
        template<class A, class B>
        static constexpr bool canIntegerContain() {
            static_assert(std::is_integral<A>::value, "SolaireCPP : canIntegerContain<A> must be integer type");
            static_assert(std::is_integral<B>::value, "SolaireCPP : canIntegerContain<B> must be integer type");
            return
                (std::numeric_limits<A>::min() <= std::numeric_limits<B>::min()) &&
                (std::numeric_limits<A>::max() >= std::numeric_limits<B>::max());
        }

        template<class A, class B, typename ENABLE = void>
        struct IntegerContainerStruct {
            typedef void Type;
        };

        template<class A, class B>
        struct IntegerContainerStruct<A, B, typename std::enable_if<canIntegerContain<A,B>()>::type> {
            typedef A Type;
        };

        template<class A, class B>
        struct IntegerContainerStruct<A, B, typename std::enable_if<canIntegerContain<B,A>() && ! canIntegerContain<A,B>()>::type> {
            typedef B Type;
        };

        template<class A, class B>
        struct IntegerContainerStruct<A, B, typename std::enable_if<
            ((! canIntegerContain<A,B>()) && (! canIntegerContain<B,A>())) &&
            (
                canIntegerContain<BiggerInteger<A>, A>() &&
                canIntegerContain<BiggerInteger<A>, B>()
             )
        >::type> {
            typedef BiggerInteger<A> Type;
        };

        template<class A, class B>
        struct IntegerContainerStruct<A, B, typename std::enable_if<
            ((! canIntegerContain<A,B>()) && (! canIntegerContain<B,A>())) &&
            (
                canIntegerContain<BiggerInteger<B>, A>() &&
                canIntegerContain<BiggerInteger<B>, B>()
             )
        >::type> {
            typedef BiggerInteger<B> Type;
        };
    }

    /*!
        \brief An integer type that can represent any value that can be represented by types A and B.
        \detail
        <table>
            <tr>
                <th>Type A</th>
                <th>Type B</th>
                <th>IntegerContainer</th>
            </tr>
            <tr>
                <td>uint16_t</td>
                <td>uint16_t</td>
                <td>uint16_t</td>
            </tr>
            <tr>
                <td>int16_t</td>
                <td>int16_t</td>
                <td>int16_t</td>
            </tr>
            <tr>
                <td>uint16_t</td>
                <td>int16_t</td>
                <td>int32_t</td>
            </tr>
            <tr>
                <td>int16_t</td>
                <td>uint16_t</td>
                <td>int32_t</td>
            </tr>
            <tr>
                <td>uint8_t</td>
                <td>uint16_t</td>
                <td>uint16_t</td>
            </tr>
        </table>
        \tparam A The first integer type.
        \tparam B The second integer type.
        \see IntegerContainer3
        \see IntegerContainer4
    */
    template<class A, class B>
    using IntegerContainer = typename Implementation::IntegerContainerStruct<A, B>::Type;

    /*!
        \brief An integer type that can represent any value that can be represented by types A, B or C.
        \tparam A The first integer type.
        \tparam B The second integer type.
        \tparam C The third integer type.
        \see IntegerContainer
        \see IntegerContainer4
    */
    template<class A, class B, class C>
    using IntegerContainer3 = IntegerContainer<IntegerContainer<A, B>, C>;

    /*!
        \brief An integer type that can represent any value that can be represented by types A, B, C or D.
        \tparam A The first integer type.
        \tparam B The second integer type.
        \tparam C The third integer type.
        \tparam D The fourth integer type.
        \see IntegerContainer
        \see IntegerContainer3
    */
    template<class A, class B, class C, class D>
    using IntegerContainer4 = IntegerContainer<IntegerContainer<A, B>, IntegerContainer<C, D>>;
}


#endif
