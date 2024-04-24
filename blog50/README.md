# BLOG50
#### Video Demo:  https://youtu.be/sYBBU1s7mBI
#### Description
This project is a colaborative blog where users can create their profiles and upload their own posts about whatever topic they want. Users are able to include images to their profile and posts. <br> Posts can be liked, which will help to rank them up in the trending page (home). <br>All the posts are currently being stored in a sqlite3 database, some server side validations are made to ensure that the content uploaded by the user is in the correct format that we expect to receive. <br>See below an overall explanation about the files, the available pages and some of its specifications.

### Files

#### /static
Contains static assets like images,favicons and css.

#### /templates
All the html templates that will rendered in the routes

#### /blog.db
The sqlite3 file that is used as the database

#### /helpers
Contains some helper functions that will be useful in the main aplication (app.py)

#### / app.py
The main application file, where all the routes are defined, data fetched and templates rendered

### Routes

#### / (homepage)
In the homepage you will see the a list of posts ranked up by date and amount of likes, the most recent and the most likes the post has, better ranked it will be. The homepage currently has the limit of 10 posts. <br>The homepage is a login required route.

#### /register
This is the page where users can create their accounts, each username must be unique in our blog, so if you provide an username that is already taken you won't be able to create an account

#### /login
As you may expect, this is the route that you use to login into your account. Your sessions are handled by flask_session.Session

#### /logout
Logout of your account.

#### /new-post
This is the place where you create new posts. Post titles must have a max length of 100 characters, and post descriptions 200. You must provide images to be used in the post preview and also in the post headings. <br>/new-post is a login required route.

#### /your-posts
A page that displays all of your posts ordered by publish date.<br>/your-posts is a login required route.

#### /post/id
The page of an individual post, it contains the content of the post itself (title, image description and text), the date it was posted, and a direct link the author's profile.<br>/post/id is a login required route.

#### /profile/username
In this route you can see users profiles in the website and its public information. If it is your own profile you can them change your profile picture by uploading a new picture with the max size of 2mb and in one of the following formats: 'png', 'jpg', 'jpeg','webp'.<br>/profile/username is a login required route.

#### /profile-picture/username
A route to get users profile pictures.<br>/profile-picture/username is a login required route.