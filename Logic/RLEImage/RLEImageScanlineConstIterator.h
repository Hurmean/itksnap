#ifndef RLEImageScanlineConstIterator_h
#define RLEImageScanlineConstIterator_h

#include "RLEImageRegionConstIterator.h"
#include "itkImageScanlineIterator.h"

namespace itk
{
/** \class ImageScanlineConstIterator
* \brief A multi-dimensional iterator templated over image type that walks a
* region of pixels, scanline by scanline or in the direction of the
* fastest axis.
*/
template< typename TPixel, typename RunLengthCounterType>
class ImageScanlineConstIterator<RLEImage<TPixel, RunLengthCounterType> >
    :public ImageRegionConstIterator<RLEImage<TPixel, RunLengthCounterType> >
{
public:
    /** Standard class typedef. */
    typedef ImageScanlineConstIterator   Self;
    typedef ImageRegionConstIterator< RLEImage<TPixel, RunLengthCounterType> > Superclass;

    /** Dimension of the image that the iterator walks.  This constant is needed so
    * functions that are templated over image iterator type (as opposed to
    * being templated over pixel type and dimension) can have compile time
    * access to the dimension of the image that the iterator walks. */
    itkStaticConstMacro(ImageIteratorDimension, unsigned int,
        Superclass::ImageIteratorDimension);

    /**
    * Index typedef support. While these were already typdef'ed in the superclass,
    * they need to be redone here for this subclass to compile properly with gcc.
    */
    /** Types inherited from the Superclass */
    typedef typename Superclass::IndexType             IndexType;
    typedef typename Superclass::SizeType              SizeType;
    typedef typename Superclass::OffsetType            OffsetType;
    typedef typename Superclass::RegionType            RegionType;
    typedef typename Superclass::ImageType             ImageType;
    typedef typename Superclass::InternalPixelType     InternalPixelType;
    typedef typename Superclass::PixelType             PixelType;

    /** Run-time type information (and related methods). */
    itkTypeMacro(ImageScanlineConstIterator, ImageRegionConstIterator);

    /** Default constructor. Needed since we provide a cast constructor. */
    ImageScanlineConstIterator()
        :ImageRegionConstIterator< ImageType >() {}

    /** Constructor establishes an iterator to walk a particular image and a
    * particular region of that image. */
    ImageScanlineConstIterator(const ImageType *ptr, const RegionType & region) :
        ImageRegionConstIterator< ImageType >(ptr, region) {}

    /** Constructor that can be used to cast from an ImageIterator to an
    * ImageScanlineConstIterator. Many routines return an ImageIterator, but for a
    * particular task, you may want an ImageScanlineConstIterator.  Rather than
    * provide overloaded APIs that return different types of Iterators, itk
    * returns ImageIterators and uses constructors to cast from an
    * ImageIterator to a ImageScanlineConstIterator. */
    ImageScanlineConstIterator(const ImageIterator< ImageType > & it)
        :ImageRegionConstIterator(it) {}

    /** Constructor that can be used to cast from an ImageConstIterator to an
    * ImageScanlineConstIterator. Many routines return an ImageIterator, but for a
    * particular task, you may want an ImageScanlineConstIterator.  Rather than
    * provide overloaded APIs that return different types of Iterators, itk
    * returns ImageIterators and uses constructors to cast from an
    * ImageIterator to a ImageScanlineConstIterator. */
    ImageScanlineConstIterator(const ImageConstIterator< ImageType > & it)
    { this->ImageRegionConstIterator< ImageType >::operator=(it); }


    /** Go to the beginning pixel of the current line.
    *
    * \sa operator++
    * \sa operator--
    * \sa NextLine
    * \sa IsAtEndOfLine
    */
    void GoToBeginOfLine(void)
    {
        this->m_Index[0] = this->m_BeginIndex[0];
        this->realIndex = 0;
        this->segmentRemainder = (*this->rlLine)[this->realIndex].first;
    }

    /** Go to the past end pixel of the current line.
    *
    * \sa GoToBeginOfLine
    * \sa operator++
    * \sa operator--
    * \sa NextLine
    * \sa IsAtEndOfLine
    */
    void GoToEndOfLine(void)
    {
        this->m_Index[0] = this->m_EndIndex[0];
        this->realIndex = this->rlLine->size() - 1;
        this->segmentRemainder = 0;
    }

    /** Test if the index is at the end of line
    */
    inline bool IsAtEndOfLine(void)
    { return this->m_Index[0] == this->m_EndIndex[0]; }

    /** Go to the next line.
    *
    * The iterator always moves to the beginning of the next line. If
    * the iterator is on the last scanline then no action is performed.
    *
    * \sa operator++
    * \sa IsAtEndOfLine
    */
    inline void NextLine(void)
    { this->Increment(); }

    /** Increment (prefix) along the scanline the iterator's index.
    *
    * If the iterator is at the end of the scanline ( one past the last
    * valid element in the row ), then the results are undefined. Which
    * means is may assert in debug mode or result in an undefined
    * iterator which may have unknown consequences if used.
    */
    Self& operator++()
    {
        itkAssertInDebugAndIgnoreInReleaseMacro(!this->IsAtEndOfLine());
        this->m_Index[0]++;
        this->segmentRemainder--;
        if (this->segmentRemainder > 0)
            return *this;

        this->realIndex++;
        this->segmentRemainder = (*this->rlLine)[this->realIndex].first;
        return *this;
    }

    /** decrement (prefix) along the scanline the iterator's index.
    *
    */
    Self& operator--()
    {
        itkAssertInDebugAndIgnoreInReleaseMacro(!this->IsAtEndOfLine());
        this->m_Index[0]--;
        this->segmentRemainder++;
        if (this->segmentRemainder <= (*this->rlLine)[this->realIndex].first)
            return *this;

        this->realIndex--;
        this->segmentRemainder = 1;
        return *this;
    }
};
} // end namespace itk

#endif //RLEImageScanlineConstIterator_h