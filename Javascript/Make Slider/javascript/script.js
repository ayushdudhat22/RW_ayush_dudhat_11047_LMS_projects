// Hero Slider

let currentSlide = 0;

const slides = document.getElementById("slides");
const totalSlides = document.querySelectorAll(".slide");

function showSlide() {

    if (currentSlide >= totalSlides.length) {
        currentSlide = 0;
    }

    if (currentSlide < 0) {
        currentSlide = totalSlides.length - 1;
    }

    slides.style.transform =
        `translateX(-${currentSlide * 100}%)`;
}

function moveSlide(n) {
    currentSlide += n;
    showSlide();
}

// Auto Hero Slider

setInterval(() => {
    currentSlide++;
    showSlide();
}, 5000);


// Top Rated Slider Buttons

function scrollLeftTop() {
    document.getElementById("topRatedRow")
        .scrollBy({
            left: -500,
            behavior: "smooth"
        });
}

function scrollRightTop() {
    document.getElementById("topRatedRow")
        .scrollBy({
            left: 500,
            behavior: "smooth"
        });
}


// Search Function

const searchInput = document.querySelector(".search input");

searchInput.addEventListener("keyup", function () {

    const value = this.value.toLowerCase();

    const cards = document.querySelectorAll(".movie-card");

    cards.forEach(card => {

        const title = card
            .querySelector("h5")
            .innerText
            .toLowerCase();

        if (title.includes(value)) {
            card.style.display = "block";
        } else {
            card.style.display = "none";
        }

    });

});


// Initial Load

showSlide();    